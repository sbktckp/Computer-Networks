/*
 * 2.1 TCP Word Count - Server (compact)
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

static int count_words(const char *s) {
    int count = 0, in_word = 0;
    for (; *s; s++) {
        if (isspace((unsigned char)*s)) in_word = 0;
        else if (!in_word) { in_word = 1; count++; }
    }
    return count;
}

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

    char sentence[1024] = {0};
    if (read(cfd, sentence, sizeof(sentence) - 1) < 0) exit(1);
    printf("\nReceived sentence:\n%s\n", sentence);

    int wc = count_words(sentence);
    printf("\nNumber of words = %d\n", wc);

    char res[32];
    snprintf(res, sizeof(res), "%d", wc);
    send(cfd, res, strlen(res), 0);
    printf("\nWord count sent to client.\n");

    close(cfd);
    close(sfd);
    return 0;
}
