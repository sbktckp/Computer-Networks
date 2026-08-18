/*
 * 4.5 String Processing (Uppercase) - Client
 * Aim: send a string and display the uppercase result.
 *
 * Input  : ./4.5_uppercase_client 127.0.0.1 8080   then type messages
 * Output : Connected to server 127.0.0.1:8080
 *          > hello
 *          Server: HELLO
 *          > quit
 *          Ending session.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
    const char *ip = argc >= 2 ? argv[1] : "127.0.0.1";
    int port = argc >= 3 ? atoi(argv[2]) : 8080;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    printf("Connected to server %s:%d\n", ip, port);
    printf("Type messages to send. Type 'quit' to end the session.\n");

    char message[1024], buffer[1024];
    for (;;) {
        printf("> ");
        fflush(stdout);
        if (!fgets(message, sizeof(message), stdin)) { printf("\nInput closed.\n"); break; }
        message[strcspn(message, "\n")] = '\0';

        send(sock, message, strlen(message), 0);
        if (strcmp(message, "quit") == 0) { printf("Ending session.\n"); break; }

        memset(buffer, 0, sizeof(buffer));
        if (read(sock, buffer, sizeof(buffer) - 1) <= 0) { printf("Server closed the connection.\n"); break; }
        printf("Server: %s\n", buffer);
    }

    close(sock);
}
