/*
 * 3.2 TCP Socket Client
 *
 * Aim: connect to a TCP server, send a fixed number of messages in a
 *      loop, print each reply, then close the connection.
 *
 * Design notes:
 *   - inet_pton() over inet_addr() to parse the server's IP: inet_addr()
 *     returns INADDR_NONE (a valid-looking value) on some malformed
 *     input, so failures can go unnoticed; inet_pton() returns a clean
 *     success/failure result and supports both IPv4 and IPv6.
 *   - The send loop matches the server's read loop: sending N messages
 *     and reading N replies demonstrates the connection being kept
 *     open across multiple exchanges, not just a single request/reply.
 *   - Command-line IP and port (with defaults) mean the same client
 *     binary can test a local server or one on another host without
 *     recompiling.
 *
 * Run:    ./client 127.0.0.1 8080
 * Output: Connected to server 127.0.0.1:8080
 *         Sent: Hello from client! (msg 1)
 *         Server: Hello from server!
 *         Sent: Hello from client! (msg 2)
 *         Server: Hello from server!
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define DEFAULT_IP    "127.0.0.1"
#define DEFAULT_PORT  8080
#define BUFFER_SIZE   1024
#define MESSAGE_COUNT 3

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

    char buffer[BUFFER_SIZE];
    const char *message = "Hello from client!";
    for (int msg_count = 1; msg_count <= MESSAGE_COUNT; msg_count++) {
        if (send(sock, message, strlen(message), 0) < 0) {
            perror("send failed");
            break;
        }
        printf("Sent: %s (msg %d)\n", message, msg_count);

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
