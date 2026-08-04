/*
 * 3.2 TCP Socket Client
 *
 * Aim: connect to a TCP server, then let the user type messages one
 *      at a time from the keyboard, sending each and printing the
 *      server's reply, until the user types "quit" or sends EOF.
 *
 * Design notes:
 *   - inet_pton() over inet_addr() to parse the server's IP: inet_addr()
 *     returns INADDR_NONE (a valid-looking value) on some malformed
 *     input, so failures can go unnoticed; inet_pton() returns a clean
 *     success/failure result and supports both IPv4 and IPv6.
 *   - fgets() reads a full line (bounded by the buffer size) instead
 *     of scanf("%s"), which would stop at the first whitespace and
 *     silently truncate any message with more than one word.
 *   - The trailing newline fgets() keeps is stripped before sending,
 *     so the server's strcmp(buffer, "quit") on the receiving end
 *     matches cleanly instead of comparing against "quit\n".
 *   - The loop exits on "quit" (matching the server's own exit
 *     condition) or on fgets() returning NULL, which happens at EOF
 *     (Ctrl+D) so the client doesn't hang if input is piped or closed.
 *
 * Run:    ./client 127.0.0.1 8080
 * Output: Connected to server 127.0.0.1:8080
 *         Type messages to send. Type 'quit' to end the session.
 *         > hi there
 *         Server: Server received: hi there
 *         > quit
 *         Ending session.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define DEFAULT_IP   "127.0.0.1"
#define DEFAULT_PORT 8080
#define BUFFER_SIZE  1024

int main(int argc, char *argv[]) {
    const char *server_ip = (argc >= 2) ? argv[1] : DEFAULT_IP;
    int port = (argc >= 3) ? atoi(argv[2]) : DEFAULT_PORT;

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

    char message[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    for (;;) {
        printf("> ");
        fflush(stdout);

        if (fgets(message, sizeof(message), stdin) == NULL) {
            printf("\nInput closed, ending session.\n");
            break;
        }

        /* Strip the trailing newline fgets() keeps, so "quit\n" still
         * compares equal to "quit" and the sent message has no stray
         * newline embedded in it. */
        message[strcspn(message, "\n")] = '\0';

        if (send(sock, message, strlen(message), 0) < 0) {
            perror("send failed");
            break;
        }

        if (strcmp(message, "quit") == 0) {
            printf("Ending session.\n");
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        ssize_t n = read(sock, buffer, sizeof(buffer) - 1);
        if (n <= 0) {
            printf("Server closed the connection.\n");
            break;
        }
        printf("Server: %s\n", buffer);
    }

    close(sock);
    return 0;
}
