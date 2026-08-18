/*
 * 1.1 TCP Calculator - Client (compact)
 * Run:    ./client 127.0.0.1 8080
 * Output: Enter first integer: 25
 *         Enter second integer: 5
 *         Enter operator (+, -, *, /): /
 *
 *         Result received from server: 5
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 3) { fprintf(stderr, "usage: %s <server_ip> <port>\n", argv[0]); exit(1); }

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sa = {0};
    sa.sin_family = AF_INET;
    sa.sin_port = htons((uint16_t)atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &sa.sin_addr);
    if (connect(fd, (struct sockaddr *)&sa, sizeof(sa)) < 0) { perror("connect"); exit(1); }

    int a, b; char op, req[1024], resp[1024] = {0};
    printf("Enter first integer: "); fflush(stdout); if (scanf("%d", &a) != 1) exit(1);
    printf("Enter second integer: "); fflush(stdout); if (scanf("%d", &b) != 1) exit(1);
    printf("Enter operator (+, -, *, /): "); fflush(stdout); if (scanf(" %c", &op) != 1) exit(1);

    snprintf(req, sizeof(req), "%d %d %c", a, b, op);
    send(fd, req, strlen(req), 0);
    if (read(fd, resp, sizeof(resp) - 1) < 0) exit(1);
    printf("\nResult received from server: %s\n", resp);

    close(fd);
    return 0;
}
