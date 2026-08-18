/*
 * 4.3 One-Way Message Transfer - Client
 * Aim: send newline-delimited messages typed at the keyboard until
 *      "quit" or EOF. No reply is read back.
 *
 * Input  : ./4.3_one_way_message_client 127.0.0.1 8080   then type
 * Output : Connected to server 127.0.0.1:8080
 *          > hello
 *          > quit
 *          Ending session.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
    const char *ip = argc >= 2 ? argv[1] : "127.0.0.1";
    int port = argc >= 3 ? atoi(argv[2]) : 8080;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    printf("Connected to server %s:%d\n", ip, port);
    printf("Type messages to send. Type 'quit' to end the session.\n");

    char message[1023], framed[1024];
    for (;;) {
        printf("> ");
        fflush(stdout);
        if (!fgets(message, sizeof(message), stdin)) { printf("\nInput closed.\n"); break; }
        message[strcspn(message, "\n")] = '\0';

        snprintf(framed, sizeof(framed), "%s\n", message);
        send(sock, framed, strlen(framed), 0);
        if (strcmp(message, "quit") == 0) { printf("Ending session.\n"); break; }
    }

    close(sock);
}
