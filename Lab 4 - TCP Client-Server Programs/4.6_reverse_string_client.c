/*
 * 4.6 Reverse a String - Client
 *
 * Aim: send a string and display the reversed string the server sends
 *      back.
 *
 * Design notes: identical to 4.4 and 4.5's client, the reversal logic
 *      lives entirely on the server; the client just sends and prints.
 *
 * Run:    ./client 127.0.0.1 8080
 * Output: Connected to server 127.0.0.1:8080
 *         Type messages to send. Type 'quit' to end the session.
 *         > hello
 *         Server: olleh
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

    char message[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    for (;;) {
        printf("> ");
        fflush(stdout);

        if (fgets(message, sizeof(message), stdin) == NULL) {
            printf("\nInput closed, ending session.\n");
            break;
        }
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
