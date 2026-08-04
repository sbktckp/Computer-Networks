/*
 * 3.1 TCP Socket Server
 * Aim: accept one TCP client, loop reading and echoing messages until
 *      the client disconnects.
 *
 * Input  : ./3.1_tcp_server 9090
 * Output : Server listening on port 9090...
 *          Connected to client: IP = 127.0.0.1, Port = 55728
 *          Client: Hello from client! (msg 1)
 *          Client disconnected.
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
    printf("Connected to client: IP = %s, Port = %d\n", client_ip, ntohs(client_addr.sin_port));

    char buffer[1024];
    for (int i = 1; ; i++) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t n = read(client_fd, buffer, sizeof(buffer) - 1);
        if (n <= 0) { printf("Client disconnected.\n"); break; }
        printf("Client: %s (msg %d)\n", buffer, i);
        send(client_fd, "Hello from server!", 18, 0);
    }

    close(client_fd);
    close(server_fd);
}
