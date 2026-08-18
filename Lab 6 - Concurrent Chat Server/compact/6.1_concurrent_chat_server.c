/*
 * 6.1 Concurrent Chat Server - Server (compact)
 * Run:    ./server
 * Output: Server is waiting for connections...
 *         Client connected: 127.0.0.1
 *         [127.0.0.1] Client: hello
 *         Server to 127.0.0.1: hi there
 *         [127.0.0.1] Client: logout
 *         Client 127.0.0.1 logged out.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

static void doprocessing(int cfd, const char *ip) {
    char buf[BUFFER_SIZE];
    while (1) {
        memset(buf, 0, BUFFER_SIZE);
        int n = recv(cfd, buf, BUFFER_SIZE - 1, 0);
        if (n <= 0) { printf("Client %s disconnected.\n", ip); break; }
        buf[n] = '\0';
        printf("[%s] Client: %s\n", ip, buf);
        if (strcmp(buf, "logout") == 0) { printf("Client %s logged out.\n", ip); break; }

        printf("Server to %s: ", ip); fflush(stdout);
        if (!fgets(buf, BUFFER_SIZE, stdin)) break;
        buf[strcspn(buf, "\n")] = '\0';
        send(cfd, buf, strlen(buf), 0);
        if (strcmp(buf, "logout") == 0) { printf("Server logged out client %s.\n", ip); break; }
    }
    close(cfd);
}

int main(void) {
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    bind(sfd, (struct sockaddr *)&addr, sizeof(addr));
    listen(sfd, 5);
    printf("Server is waiting for connections...\n");
    signal(SIGCHLD, SIG_IGN);

    while (1) {
        struct sockaddr_in ca; socklen_t cl = sizeof(ca);
        int cfd = accept(sfd, (struct sockaddr *)&ca, &cl);
        if (cfd < 0) continue;

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &ca.sin_addr, ip, sizeof(ip));
        printf("Client connected: %s\n", ip);

        pid_t pid = fork();
        if (pid < 0) { close(cfd); continue; }
        if (pid == 0) {
            close(sfd);
            doprocessing(cfd, ip);
            exit(0);
        } else {
            close(cfd);
        }
    }

    close(sfd);
    return 0;
}
