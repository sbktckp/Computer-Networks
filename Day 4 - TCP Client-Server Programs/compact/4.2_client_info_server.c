/*
 * 4.2 Display Client Information - Server
 * Aim: accept one client and display its IP and port.
 *
 * Input  : ./4.2_client_info_server 8080
 * Output : Server listening on port 8080...
 *          Client connected: IP = 127.0.0.1, Port = 54321
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

    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    printf("Client connected: IP = %s, Port = %d\n", client_ip, ntohs(client_addr.sin_port));

    close(client_fd);
    close(server_fd);
}
