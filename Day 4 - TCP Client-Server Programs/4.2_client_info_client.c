/*
 * 4.2 Display Client Information - Client
 *
 * Aim: accept the server's IP and port from the command line and
 *      connect, so the server has a live client to report on.
 *
 * Design notes: identical connection setup to 4.1, this question's
 *      point of interest is entirely on the server side (reading back
 *      the client's own address via accept()); the client just needs
 *      to exist and stay connected briefly.
 *
 * Run:    ./client 127.0.0.1 8080
 * Output: Connected to server 127.0.0.1:8080
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

    printf("Connected to server %s:%d\n", server_ip, port);

    close(sock);
    return 0;
}
