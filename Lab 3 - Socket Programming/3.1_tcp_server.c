/*
 * 3.1 TCP Socket Server
 *
 * Aim: create a TCP server that accepts one client connection, then
 *      exchanges messages with it for as long as the client keeps
 *      sending them, until the client disconnects or sends "quit".
 *
 * Design notes:
 *   - socket() -> bind() -> listen() -> accept() is the fixed TCP
 *     server sequence; each step's return value is checked because a
 *     silent failure here (e.g. bind() failing on a port already in
 *     use) would otherwise surface as a confusing hang at accept().
 *   - SO_REUSEADDR is set before bind() so the server can restart on
 *     the same port immediately; without it, a recently closed socket
 *     leaves the port in TIME_WAIT and the next bind() fails.
 *   - The read/reply loop keeps going until read() returns 0 (client
 *     closed cleanly), a negative value (error), or the client sends
 *     the literal string "quit", so the session length is driven by
 *     the client typing messages, not by a fixed message count.
 *   - inet_ntop() recovers the client's IP as a string for logging;
 *     the address arrives from accept() only in binary form.
 *
 * Run:    ./server 8080
 * Output: Server listening on port 8080...
 *         Connected to client: IP = 127.0.0.1, Port = <ephemeral>
 *         Client: hi there
 *         Client: quit
 *         Client requested to end the session.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define DEFAULT_PORT 8080
#define BUFFER_SIZE  1024

int main(int argc, char *argv[]) {
    int port = (argc == 2) ? atoi(argv[1]) : DEFAULT_PORT;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    /* Lets the server rebind to this port immediately after a restart. */
    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

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
    printf("Connected to client: IP = %s, Port = %d\n",
           client_ip, ntohs(client_addr.sin_port));

    /* Keep exchanging messages for as long as the client sends them. */
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

        char reply[BUFFER_SIZE];
        snprintf(reply, sizeof(reply), "Server received: %s", buffer);
        if (send(client_fd, reply, strlen(reply), 0) < 0) {
            perror("send failed");
            break;
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
