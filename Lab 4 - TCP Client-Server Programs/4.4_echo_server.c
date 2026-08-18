/*
 * 4.4 Echo Server - Server
 *
 * Aim: receive a message from the client and send the same message
 *      straight back, unchanged.
 *
 * Design notes:
 *   - This is the two-way counterpart to 4.3: the server now calls
 *     send() after every read(), so the client can display what came
 *     back. The transformation applied here is the identity function,
 *     Q5 and Q6 in this set reuse this exact read/transform/send shape
 *     with uppercase and reverse in place of "send it back as-is".
 *   - Echoing the same buffer straight back needs no extra copy: the
 *     bytes read() already sitting in buffer are exactly what send()
 *     needs to write out.
 *
 * Run:    ./server 8080
 * Output: Server listening on port 8080...
 *         Client connected: IP = 127.0.0.1, Port = 54321
 *         Client: hello
 *         Client: quit
 *         Client requested to end the session.
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

    printf("Server listening on port %d...\n", port);

    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    printf("Client connected: IP = %s, Port = %d\n",
           client_ip, ntohs(client_addr.sin_port));

    char buffer[BUFFER_SIZE];
    for (;;) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t n = read(client_fd, buffer, sizeof(buffer) - 1);
        if (n < 0) {
            perror("read failed");
            break;
        }
        if (n == 0) {
            printf("Client disconnected.\n");
            break;
        }

        printf("Client: %s\n", buffer);

        if (strcmp(buffer, "quit") == 0) {
            printf("Client requested to end the session.\n");
            break;
        }

        /* Echo: the bytes just read are sent back unmodified. */
        if (send(client_fd, buffer, (size_t)n, 0) < 0) {
            perror("send failed");
            break;
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
