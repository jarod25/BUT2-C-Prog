#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char * argv []) {

    if (argc < 3) {
        fprintf(stderr, "Erreur au - 2 argmuments attendus mais %i fournis\n", argc);
        return EXIT_FAILURE;
    }

    char buf [argc - 2];
    int port = atoi(argv[1]), sock, bufIndex = 0;
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = htonl(INADDR_ANY)
    };

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Erreur socket ");
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < argc-2; i += 1) {
        stpcpy(&buf[bufIndex], argv[i+2]);
        bufIndex += strlen(&buf[bufIndex]);
    }

    if (sendto(sock, &bufIndex, sizeof bufIndex, 0, (struct sockaddr *) &addr, sizeof addr) != sizeof bufIndex) {
        perror("Erreur sendto ");
        return EXIT_FAILURE;
    }

    if (sendto(sock, buf, bufIndex, 0, (struct sockaddr *) &addr, sizeof addr) != (ssize_t)bufIndex) {
        perror("Erreur sendto ");
        return EXIT_FAILURE;
    }

    char resp [35];
    if (recvfrom(sock, resp, sizeof resp, 0, NULL, NULL) < 0) {
        perror("Erreur recvfrom ");
        return EXIT_FAILURE;
    }

    if (write(STDOUT_FILENO, resp, sizeof resp) != (int)(sizeof resp)) {
        perror("Erreur write ");
        return EXIT_FAILURE;
    }

    fprintf(stdout, "\n");

    return EXIT_SUCCESS;
}