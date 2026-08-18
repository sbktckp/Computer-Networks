/*
 * 4.1 Basic TCP Connection - Server
 * Aim: accept a port, bind, listen, accept one client, print success.
 *
 * Input  : ./4.1_basic_connection_server 8080
 * Output : Server listening on port 8080...
 *          Client Connected Successfully.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
    int port = argc == 2 ? atoi(argv[1]) : 8080;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons((uint16_t)port);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 3);
    printf("Server listening on port %d...\n", port);

    int client_fd = accept(server_fd, NULL, NULL);
    printf("Client Connected Successfully.\n");

    close(client_fd);
    close(server_fd);
}
