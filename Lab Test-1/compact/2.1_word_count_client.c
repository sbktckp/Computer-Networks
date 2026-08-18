/*
 * 2.1 TCP Word Count - Client (compact)
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

int main(int argc, char *argv[]) {
    if (argc != 3) { fprintf(stderr, "usage: %s <server_ip> <port>\n", argv[0]); exit(1); }

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sa = {0};
    sa.sin_family = AF_INET;
    sa.sin_port = htons((uint16_t)atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &sa.sin_addr);
    if (connect(fd, (struct sockaddr *)&sa, sizeof(sa)) < 0) { perror("connect"); exit(1); }
    printf("Connected to server.\n");

    char line[1024], resp[1024] = {0};
    printf("\nEnter a sentence:\n");
    if (!fgets(line, sizeof(line), stdin)) exit(1);
    line[strcspn(line, "\n")] = '\0';

    send(fd, line, strlen(line), 0);
    if (read(fd, resp, sizeof(resp) - 1) < 0) exit(1);
    printf("\nWord count received from server: %s\n", resp);

    close(fd);
    return 0;
}
