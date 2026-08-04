/*
 * 4.1 Basic TCP Connection - Server
 *
 * Aim: accept a port number from the command line, create a TCP
 *      socket, bind it, listen, accept exactly one client, print a
 *      success message, then close.
 *
 * Design notes:
 *   - This is the minimal TCP server skeleton every later question in
 *     this set builds on: socket() -> bind() -> listen() -> accept().
 *     No message exchange happens here; the point is just proving a
 *     connection can be established, so once accept() returns the
 *     server has nothing left to do but report success and exit.
 *   - SO_REUSEADDR is set before bind() so re-running the server right
 *     after stopping it doesn't fail with "Address already in use";
 *     without it, the OS holds the port in TIME_WAIT briefly after
 *     close.
 *
 * Run:    ./server 8080
 * Output: Server listening on port 8080...
 *         Client Connected Successfully.
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

    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client Connected Successfully.\n");

    close(client_fd);
    close(server_fd);
    return 0;
}
