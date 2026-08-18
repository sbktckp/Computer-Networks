/*
 * 5.1 TCP Chat Application - Server (compact)
 * Run:    ./server
 * Output: Server is waiting for connection...
 *         Client connected.
 *         Client: hello
 *         Server: hi there
 *         Client: logout
 *         Client logged out.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(void) {
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    if (bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) { perror("Bind failed"); exit(1); }
    listen(sfd, 5);
    printf("Server is waiting for connection...\n");

    struct sockaddr_in ca; socklen_t cl = sizeof(ca);
    int cfd = accept(sfd, (struct sockaddr *)&ca, &cl);
    printf("Client connected.\n");

    char buf[BUFFER_SIZE];
    while (1) {
        memset(buf, 0, BUFFER_SIZE);
        int n = recv(cfd, buf, BUFFER_SIZE - 1, 0);
        if (n <= 0) { printf("Client disconnected.\n"); break; }
        buf[n] = '\0';
        printf("Client: %s\n", buf);
        if (strcmp(buf, "logout") == 0) { printf("Client logged out.\n"); break; }

        printf("Server: "); fflush(stdout);
        if (!fgets(buf, BUFFER_SIZE, stdin)) break;
        buf[strcspn(buf, "\n")] = '\0';
        send(cfd, buf, strlen(buf), 0);
        if (strcmp(buf, "logout") == 0) { printf("Server logged out.\n"); break; }
    }

    close(cfd);
    close(sfd);
    return 0;
}
