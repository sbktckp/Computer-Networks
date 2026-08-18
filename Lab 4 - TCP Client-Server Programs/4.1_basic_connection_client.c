/*
 * 4.1 Basic TCP Connection - Client
 *
 * Aim: accept the server's IP and port from the command line, connect
 *      to it, print a success message, then close.
 *
 * Design notes:
 *   - inet_pton() over inet_addr() to parse the server IP: inet_addr()
 *     returns INADDR_NONE, a valid-looking value, on some malformed
 *     input, so a bad address can silently pass through; inet_pton()
 *     has an unambiguous success/failure return.
 *   - connect() performs the TCP three-way handshake; a nonzero return
 *     means the server never accepted the connection, worth
 *     distinguishing from a successful connect with a clear message.
 *
 * Run:    ./client 127.0.0.1 8080
 * Output: Connected to Server Successfully.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

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

    printf("Connected to Server Successfully.\n");

    close(sock);
    return 0;
}
