/*
 * 4.3 One-Way Message Transfer - Server
 *
 * Aim: receive and display text messages from the client. No reply is
 *      sent back; the client only sends, the server only reads.
 *
 * Design notes:
 *   - This is genuinely one-way at the socket level: send() is only
 *     called on the client side, read() only on the server side. It
 *     is not "echo without printing the reply", the server never
 *     calls send() at all here.
 *   - TCP is a byte stream with no built-in message boundaries, so
 *     two messages sent back to back can arrive in a single read()
 *     (e.g. "hello" and "quit" merging into "helloquit"). Since there
 *     is no reply to pace the client's sends here, unlike 4.4-4.6,
 *     this server frames messages explicitly: the client appends a
 *     newline after each message, and read_line() below reads one
 *     byte at a time until it sees that newline, so a message is
 *     never mixed with the one that follows it, no matter how TCP
 *     happens to batch the underlying packets.
 *   - The loop keeps reading messages until the client disconnects or
 *     sends "quit", the same session-driven-by-the-client pattern used
 *     in Lab 3, so the number of messages is not fixed in advance.
 *
 * Run:    ./server 8080
 * Output: Server listening on port 8080...
 *         Client connected: IP = 127.0.0.1, Port = 54321
 *         Client: hello there
 *         Client: quit
 *         Client requested to end the session.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

/* Reads one newline-delimited message into buf (without the newline).
 * Returns the message length on success, 0 on clean disconnect, -1 on
 * error. Reading a single byte at a time is not the most throughput-
 * efficient framing technique, but it is the simplest one to reason
 * about correctly, and this exercise sends short interactive lines,
 * not bulk data, so the extra read() calls cost nothing noticeable. */
static ssize_t read_line(int fd, char *buf, size_t max_len) {
    size_t i = 0;
    while (i < max_len - 1) {
        char c;
        ssize_t n = read(fd, &c, 1);
        if (n < 0) return -1;
        if (n == 0) return (i == 0) ? 0 : (ssize_t)i;
        if (c == '\n') break;
        buf[i++] = c;
    }
    buf[i] = '\0';
    return (ssize_t)i;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int port = atoi(argv[1]);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int reuse = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons((uint16_t)port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", port);

    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    printf("Client connected: IP = %s, Port = %d\n",
           client_ip, ntohs(client_addr.sin_port));

    char buffer[BUFFER_SIZE];
    for (;;) {
        ssize_t n = read_line(client_fd, buffer, sizeof(buffer));
        if (n < 0) {
            perror("read failed");
            break;
        }
        if (n == 0) {
            printf("Client disconnected.\n");
            break;
        }

        printf("Client: %s\n", buffer);

        if (strcmp(buffer, "quit") == 0) {
            printf("Client requested to end the session.\n");
            break;
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
