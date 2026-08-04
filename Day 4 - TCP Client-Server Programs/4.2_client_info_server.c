/*
 * 4.2 Display Client Information - Server
 *
 * Aim: accept one client and display its IP address and port number.
 *
 * Design notes:
 *   - accept() fills a struct sockaddr_in with the client's address
 *     when passed a non-NULL pointer (question 4.1 passed NULL since
 *     it didn't need this). The address arrives in binary form, so
 *     inet_ntop() converts it to a printable string.
 *   - The port inside that struct is in network byte order regardless
 *     of the host's own endianness (see Day 2's endianness material),
 *     so ntohs() converts it back to a normal host-order int before
 *     printing; printing sin_port directly would show the wrong number
 *     on a little-endian machine.
 *
 * Run:    ./server 8080
 * Output: Server listening on port 8080...
 *         Client connected: IP = 127.0.0.1, Port = 54321
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

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

    close(client_fd);
    close(server_fd);
    return 0;
}
