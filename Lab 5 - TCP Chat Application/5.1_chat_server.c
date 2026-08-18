/*
 * 5.1 TCP Chat Application - Server
 *
 * Aim: hold an interactive back-and-forth chat with one client over a
 *      single TCP connection, until either side types "logout".
 *
 * Design notes:
 *   - Unlike a one-shot request/response, this loops receive-then-send
 *     indefinitely, so the same connection carries many messages
 *     instead of a fresh connection per message.
 *   - "logout" is checked on both sides of the loop: if the client
 *     sends it, the server stops without sending a reply; if the
 *     server operator types it, the server sends it (so the client's
 *     own check can react) and then stops.
 *   - recv() returning 0 or a negative value means the client closed
 *     the socket or dropped the connection; that ends the chat exactly
 *     like an explicit logout would.
 *
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
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port        = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(1);
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(1);
    }

    printf("Server is waiting for connection...\n");

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(1);
    }

    printf("Client connected.\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int n = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (n <= 0) {
            printf("Client disconnected.\n");
            break;
        }
        buffer[n] = '\0';
        printf("Client: %s\n", buffer);

        if (strcmp(buffer, "logout") == 0) {
            printf("Client logged out.\n");
            break;
        }

        printf("Server: ");
        fflush(stdout);
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        send(client_fd, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "logout") == 0) {
            printf("Server logged out.\n");
            break;
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
