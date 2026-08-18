/*
 * 6.1 Concurrent Chat Server - Server
 *
 * Aim: accept many clients at once, each on its own child process
 *      created with fork(), and hold an interactive chat with each
 *      one independently until that client sends "logout".
 *
 * Design notes:
 *   - The parent's only job is accept() + fork(): it never touches
 *     client_fd itself, closing its copy right after forking so the
 *     child holds the only open reference and the socket closes for
 *     real once that child is done.
 *   - The child gets a duplicate of server_fd too (fork() copies all
 *     open descriptors), so it closes that one immediately: a child
 *     accepting new connections on the listening socket would race
 *     the parent for them.
 *   - fork() returning 0 identifies the child; returning the child's
 *     PID (or -1 on failure) identifies the parent, so a single
 *     if/else after one fork() call is enough to send each process
 *     down its own path.
 *   - "logout" ends only that child's loop and process (exit(0) after
 *     doprocessing() returns), so other children serving other
 *     clients are completely unaffected.
 *   - inet_ntop() converts the client's binary IP address into the
 *     dotted-decimal string printed alongside every message, so the
 *     operator can tell which client said what even with several
 *     chats running concurrently.
 *
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
#include <sys/wait.h>
#include <signal.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

static void doprocessing(int client_fd, const char *client_ip) {
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int n = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (n <= 0) {
            printf("Client %s disconnected.\n", client_ip);
            break;
        }
        buffer[n] = '\0';
        printf("[%s] Client: %s\n", client_ip, buffer);

        if (strcmp(buffer, "logout") == 0) {
            printf("Client %s logged out.\n", client_ip);
            break;
        }

        printf("Server to %s: ", client_ip);
        fflush(stdout);
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        send(client_fd, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "logout") == 0) {
            printf("Server logged out client %s.\n", client_ip);
            break;
        }
    }

    close(client_fd);
}

int main(void) {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pid_t pid;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    int reuse = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

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

    printf("Server is waiting for connections...\n");

    /* Reap finished children as they exit, instead of leaving zombies
     * around: SIG_IGN on SIGCHLD tells the kernel to auto-reap. */
    signal(SIGCHLD, SIG_IGN);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
        printf("Client connected: %s\n", client_ip);

        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            close(client_fd);
            continue;
        }

        if (pid == 0) {
            /* Child: serves this one client, then exits. */
            close(server_fd);
            doprocessing(client_fd, client_ip);
            exit(0);
        } else {
            /* Parent: hands the connection off to the child and goes
             * back to accept()ing the next one. */
            close(client_fd);
        }
    }

    close(server_fd);
    return 0;
}
