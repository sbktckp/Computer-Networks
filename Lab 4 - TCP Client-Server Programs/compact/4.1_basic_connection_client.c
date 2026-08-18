/*
 * 4.1 Basic TCP Connection - Client
 * Aim: connect to a TCP server and print success.
 *
 * Input  : ./4.1_basic_connection_client 127.0.0.1 8080
 * Output : Connected to Server Successfully.
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
    printf("Connected to Server Successfully.\n");

    close(sock);
}
