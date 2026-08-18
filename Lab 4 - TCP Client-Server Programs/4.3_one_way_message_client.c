/*
 * 4.3 One-Way Message Transfer - Client
 *
 * Aim: read messages from the keyboard and send each one to the
 *      server. No reply is read back, since the server never sends one.
 *
 * Design notes:
 *   - fgets() over scanf("%s") so a multi-word message isn't silently
 *     truncated at the first space.
 *   - The trailing newline fgets() keeps is stripped off with
 *     strcspn(message, "\n") first, then a single '\n' is appended
 *     back on before send(): stripping first guarantees exactly one
 *     newline goes out (fgets() might not have captured one at all if
 *     the line was exactly BUFFER_SIZE-1 bytes with no room left), and
 *     appending it explicitly is what lets the server's read_line()
 *     find a message boundary in the byte stream.
 *   - No read() call anywhere in this client; attempting one would
 *     just block forever, since the server in this question never
 *     calls send(). That absence is the entire point of "one-way".
 *
 * Run:    ./client 127.0.0.1 8080
 * Output: Connected to server 127.0.0.1:8080
 *         Type messages to send. Type 'quit' to end the session.
 *         > hello there
 *         > quit
 *         Ending session.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <server_ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const char *server_ip = argv[1];
    int port = atoi(argv[2]);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons((uint16_t)port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "invalid server address: %s\n", server_ip);
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server %s:%d\n", server_ip, port);
    printf("Type messages to send. Type 'quit' to end the session.\n");

    /* One byte smaller than the frame buffer, so appending "\n" in
     * framed[] below can never be truncated regardless of how much
     * of the line fgets() captured. */
    char message[BUFFER_SIZE - 1];
    for (;;) {
        printf("> ");
        fflush(stdout);

        if (fgets(message, sizeof(message), stdin) == NULL) {
            printf("\nInput closed, ending session.\n");
            break;
        }
        message[strcspn(message, "\n")] = '\0';

        char framed[BUFFER_SIZE];
        snprintf(framed, sizeof(framed), "%s\n", message);

        if (send(sock, framed, strlen(framed), 0) < 0) {
            perror("send failed");
            break;
        }

        if (strcmp(message, "quit") == 0) {
            printf("Ending session.\n");
            break;
        }
    }

    close(sock);
    return 0;
}
