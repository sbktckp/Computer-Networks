/*
 * 1.1 TCP Calculator - Server (compact)
 * Run:    ./server 8080
 * Output: Server is waiting for a client...
 *         Client connected.
 *
 *         Received:
 *         First integer  = 25
 *         Second integer = 5
 *         Operator       = /
 *
 *         Result = 5
 *
 *         Result sent to client.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 2) { fprintf(stderr, "usage: %s <port>\n", argv[0]); exit(1); }

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons((uint16_t)atoi(argv[1]));
    bind(sfd, (struct sockaddr *)&addr, sizeof(addr));
    listen(sfd, 3);
    printf("Server is waiting for a client...\n");

    struct sockaddr_in ca; socklen_t cl = sizeof(ca);
    int cfd = accept(sfd, (struct sockaddr *)&ca, &cl);
    printf("Client connected.\n");

    char buf[1024] = {0};
    if (read(cfd, buf, sizeof(buf) - 1) < 0) exit(1);
    int a = 0, b = 0; char op = '\0';
    sscanf(buf, "%d %d %c", &a, &b, &op);
    printf("\nReceived:\nFirst integer  = %d\nSecond integer = %d\nOperator       = %c\n", a, b, op);

    char res[1024]; int err = 0;
    switch (op) {
        case '+': snprintf(res, sizeof(res), "%d", a + b); break;
        case '-': snprintf(res, sizeof(res), "%d", a - b); break;
        case '*': snprintf(res, sizeof(res), "%d", a * b); break;
        case '/':
            if (b == 0) { snprintf(res, sizeof(res), "Error: division by zero"); err = 1; }
            else snprintf(res, sizeof(res), "%d", a / b);
            break;
        default: snprintf(res, sizeof(res), "Error: invalid operator"); err = 1;
    }
    printf(err ? "\n%s\n" : "\nResult = %s\n", res);

    send(cfd, res, strlen(res), 0);
    printf("\nResult sent to client.\n");

    close(cfd);
    close(sfd);
    return 0;
}
