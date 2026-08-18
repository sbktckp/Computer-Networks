/*
 * Task-1 TCP Calculator - Client
 *
 * Aim: send two integers and an arithmetic operator to the server,
 *      then display the result the server computes and sends back.
 *
 * Design notes:
 *   - The three values are packed into one line ("25 5 /") so a single
 *     send()/read() pair carries the whole request; the server just
 *     sscanf()s it apart instead of needing three separate messages.
 *   - The client does no arithmetic itself. It only formats the
 *     request and prints whatever the server returns, so division by
 *     zero is handled once, on the server, rather than in both places.
 *
 * Run:    ./client 127.0.0.1 8080
 * Output: Enter first integer: 25
 *         Enter second integer: 5
 *         Enter operator (+, -, *, /): /
 *
 *         Result received from server: 5
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

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons((uint16_t)port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "invalid address: %s\n", server_ip);
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    int a, b;
    char op;

    printf("Enter first integer: ");
    fflush(stdout);
    if (scanf("%d", &a) != 1) {
        fprintf(stderr, "invalid input\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Enter second integer: ");
    fflush(stdout);
    if (scanf("%d", &b) != 1) {
        fprintf(stderr, "invalid input\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Enter operator (+, -, *, /): ");
    fflush(stdout);
    if (scanf(" %c", &op) != 1) {
        fprintf(stderr, "invalid input\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    char request[BUFFER_SIZE];
    snprintf(request, sizeof(request), "%d %d %c", a, b, op);

    if (send(sock_fd, request, strlen(request), 0) < 0) {
        perror("send failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    char response[BUFFER_SIZE];
    memset(response, 0, sizeof(response));
    ssize_t n = read(sock_fd, response, sizeof(response) - 1);
    if (n < 0) {
        perror("read failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("\nResult received from server: %s\n", response);

    close(sock_fd);
    return 0;
}
