#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SIZEMAX 4026

int main (int argc, char * argv []) {

    if (argc != 2) {
        fprintf(stderr, "%i arguments\n", argc);
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]), rd, sock, tailleBuf;
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = htonl(INADDR_ANY)
    };
    struct sockaddr_in src;
    socklen_t size = sizeof src;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Erreur socket ");
        return EXIT_FAILURE;
    }

    if (bind(sock, (struct sockaddr *) &addr, sizeof addr) < 0) {
        perror("Erreur bind ");
        return EXIT_FAILURE;
    }

    if (recvfrom(sock, &tailleBuf, sizeof tailleBuf, 0, (struct sockaddr *) &src, &size) != sizeof tailleBuf) {
        perror("Erreur recvfrom ");
        return EXIT_FAILURE;
    }

    char buf [tailleBuf];
    if ((rd = recvfrom(sock, buf, tailleBuf, 0, (struct sockaddr *) &src, &size)) < 0) {
        perror("Erreur recvfrom ");
        return EXIT_FAILURE;
    }

    if (write(STDOUT_FILENO, buf, rd) != rd) {
        perror("Erreur write ");
        return EXIT_FAILURE;
    }
    
    fprintf(stdout, "\n");

    char mess [35];
    sprintf(mess, "lu un message de %i octets", rd);
    if (sendto(sock, mess, sizeof mess, 0, (struct sockaddr *) &src, sizeof src) != (ssize_t)(sizeof mess)) {
        perror("Erreur sendto ");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}