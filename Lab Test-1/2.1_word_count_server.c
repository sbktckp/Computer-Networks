/*
 * Task-2 TCP Word Count - Server
 *
 * Aim: receive a sentence from the client, count how many words it
 *      contains, print the sentence and count, then send the count
 *      back to the client.
 *
 * Design notes:
 *   - A "word" is any run of non-space characters. Walking the buffer
 *     once and toggling an in_word flag on space/non-space transitions
 *     counts words correctly regardless of leading, trailing, or
 *     repeated spaces, without needing strtok() to mutate the buffer.
 *   - An empty (or all-space) sentence naturally counts to 0, since
 *     the flag never flips into "in a word" if no non-space byte is
 *     ever seen.
 *
 * Run:    ./server 8080
 * Output: Server is waiting for a client...
 *         Client connected.
 *
 *         Received sentence:
 *         TCP socket programming is interesting
 *
 *         Number of words = 5
 *
 *         Word count sent to client.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

static int count_words(const char *s) {
    int count = 0;
    int in_word = 0;

    for (; *s != '\0'; s++) {
        if (isspace((unsigned char)*s)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            count++;
        }
    }
    return count;
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

    printf("Server is waiting for a client...\n");

    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    char sentence[BUFFER_SIZE];
    memset(sentence, 0, sizeof(sentence));
    ssize_t n = read(client_fd, sentence, sizeof(sentence) - 1);
    if (n < 0) {
        perror("read failed");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("\nReceived sentence:\n%s\n", sentence);

    int word_count = count_words(sentence);
    printf("\nNumber of words = %d\n", word_count);

    char result_str[32];
    snprintf(result_str, sizeof(result_str), "%d", word_count);

    if (send(client_fd, result_str, strlen(result_str), 0) < 0) {
        perror("send failed");
    } else {
        printf("\nWord count sent to client.\n");
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
