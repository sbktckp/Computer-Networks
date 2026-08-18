/*
 * 6.1 Concurrent Chat Server - Client
 *
 * Aim: connect to the concurrent chat server and hold an interactive
 *      chat, identical in protocol to a single-client chat, so several
 *      copies of this client can run at once against the same server.
 *
 * Design notes:
 *   - Nothing here is aware that the server is concurrent: fork()
 *     happens entirely on the server side, so this client is exactly
 *     what a plain single-client chat client looks like. That is the
 *     point of the assignment, concurrency is invisible from a single
 *     client's point of view, each gets its own private conversation.
 *   - Running two or more instances of this program (in separate
 *     terminals) against the same server demonstrates that a "logout"
 *     from one client does not disturb the others.
 *
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
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(1);
    }

    printf("Connected to server.\n");

    while (1) {
        printf("Client: ");
        fflush(stdout);
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        send(sock, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "logout") == 0) {
            printf("Client logged out.\n");
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        int n = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (n <= 0) {
            printf("Server disconnected.\n");
            break;
        }
        buffer[n] = '\0';
        printf("Server: %s\n", buffer);

        if (strcmp(buffer, "logout") == 0) {
            printf("Server logged out.\n");
            break;
        }
    }

    close(sock);
    return 0;
}
