/*
 * 5.1 TCP Chat Application - Client (compact)
 * Run:    ./client
 * Output: Connected to server.
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
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sa = {0};
    sa.sin_family = AF_INET;
    sa.sin_port = htons(PORT);
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(fd, (struct sockaddr *)&sa, sizeof(sa)) < 0) { perror("Connection failed"); exit(1); }
    printf("Connected to server.\n");

    char buf[BUFFER_SIZE];
    while (1) {
        printf("Client: "); fflush(stdout);
        if (!fgets(buf, BUFFER_SIZE, stdin)) break;
        buf[strcspn(buf, "\n")] = '\0';
        send(fd, buf, strlen(buf), 0);
        if (strcmp(buf, "logout") == 0) { printf("Client logged out.\n"); break; }

        memset(buf, 0, BUFFER_SIZE);
        int n = recv(fd, buf, BUFFER_SIZE - 1, 0);
        if (n <= 0) { printf("Server disconnected.\n"); break; }
        buf[n] = '\0';
        printf("Server: %s\n", buf);
        if (strcmp(buf, "logout") == 0) { printf("Server logged out.\n"); break; }
    }

    close(fd);
    return 0;
}
