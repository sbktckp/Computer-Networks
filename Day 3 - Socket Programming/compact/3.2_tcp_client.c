/*
 * 3.2 TCP Socket Client
 * Aim: connect to a TCP server, send 3 messages in a loop, print each
 *      reply.
 *
 * Input  : ./3.2_tcp_client 127.0.0.1 9090
 * Output : Connected to server 127.0.0.1:9090
 *          Sent: Hello from client! (msg 1)
 *          Server: Hello from server!
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
    const char *ip = argc >= 2 ? argv[1] : "127.0.0.1";
    int port = argc >= 3 ? atoi(argv[2]) : 8080;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    printf("Connected to server %s:%d\n", ip, port);

    char buffer[1024];
    for (int i = 1; i <= 3; i++) {
        send(sock, "Hello from client!", 18, 0);
        printf("Sent: Hello from client! (msg %d)\n", i);
        memset(buffer, 0, sizeof(buffer));
        if (read(sock, buffer, sizeof(buffer) - 1) <= 0) break;
        printf("Server: %s\n", buffer);
    }

    close(sock);
}
