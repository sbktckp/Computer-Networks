/*
 * Task-1 TCP Calculator - Server
 *
 * Aim: receive two integers and an operator from the client, perform
 *      the requested arithmetic, print the operands and the result,
 *      then send the result back to the client.
 *
 * Design notes:
 *   - sscanf() pulls all three fields out of the one line the client
 *     sent, so there is no need to frame or delimit the request beyond
 *     the whitespace already separating "25 5 /".
 *   - Division by zero is checked explicitly before the division
 *     happens; the server replies with an error message instead of
 *     letting the process crash on SIGFPE.
 *   - Result is sent back to the client as text (e.g. "5"), not a raw
 *     int, so the client can print it directly without conversion.
 *
 * Run:    ./server 8080
 * Output: Server is waiting for a client...
 *         Client connected.
 *
 *         Received:
 *         First integer  = 25
 *         Second integer = 5
 *         Operator       = /
 *
 *         Result = 5
 *
 *         Result sent to client.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int port = atoi(argv[1]);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int reuse = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons((uint16_t)port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is waiting for a client...\n");

    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    ssize_t n = read(client_fd, buffer, sizeof(buffer) - 1);
    if (n < 0) {
        perror("read failed");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    int a = 0, b = 0;
    char op = '\0';
    sscanf(buffer, "%d %d %c", &a, &b, &op);

    printf("\nReceived:\n");
    printf("First integer  = %d\n", a);
    printf("Second integer = %d\n", b);
    printf("Operator       = %c\n", op);

    char result_str[BUFFER_SIZE];
    int has_error = 0;

    switch (op) {
        case '+':
            snprintf(result_str, sizeof(result_str), "%d", a + b);
            break;
        case '-':
            snprintf(result_str, sizeof(result_str), "%d", a - b);
            break;
        case '*':
            snprintf(result_str, sizeof(result_str), "%d", a * b);
            break;
        case '/':
            if (b == 0) {
                snprintf(result_str, sizeof(result_str), "Error: division by zero");
                has_error = 1;
            } else {
                snprintf(result_str, sizeof(result_str), "%d", a / b);
            }
            break;
        default:
            snprintf(result_str, sizeof(result_str), "Error: invalid operator");
            has_error = 1;
            break;
    }

    if (has_error) {
        printf("\n%s\n", result_str);
    } else {
        printf("\nResult = %s\n", result_str);
    }

    if (send(client_fd, result_str, strlen(result_str), 0) < 0) {
        perror("send failed");
    } else {
        printf("\nResult sent to client.\n");
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
