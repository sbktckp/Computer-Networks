/*
 * Task-2 TCP Word Count - Client
 *
 * Aim: send a sentence to the server and display the word count the
 *      server sends back.
 *
 * Design notes:
 *   - fgets() reads the whole line, spaces included, which a plain
 *     scanf("%s", ...) could not do; the trailing newline it leaves
 *     behind is stripped before the sentence goes over the socket.
 *   - Counting happens entirely on the server; the client's only job
 *     is to transport the sentence and print whatever count comes
 *     back, so leading/trailing/multiple-space handling lives in one
 *     place.
 *
 * Run:    ./client 127.0.0.1 8080
 * Output: Connected to server.
 *
 *         Enter a sentence:
 *         TCP socket programming is interesting
 *
 *         Word count received from server: 5
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <server_ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const char *server_ip = argv[1];
    int port = atoi(argv[2]);

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons((uint16_t)port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "invalid address: %s\n", server_ip);
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    char sentence[BUFFER_SIZE];
    printf("\nEnter a sentence:\n");
    if (fgets(sentence, sizeof(sentence), stdin) == NULL) {
        fprintf(stderr, "invalid input\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    sentence[strcspn(sentence, "\n")] = '\0';

    if (send(sock_fd, sentence, strlen(sentence), 0) < 0) {
        perror("send failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    char response[BUFFER_SIZE];
    memset(response, 0, sizeof(response));
    ssize_t n = read(sock_fd, response, sizeof(response) - 1);
    if (n < 0) {
        perror("read failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("\nWord count received from server: %s\n", response);

    close(sock_fd);
    return 0;
}
