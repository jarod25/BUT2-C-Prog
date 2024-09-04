#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: Nom de la machine, port\n");
        exit(1);
    }
    char *host = argv[1];
    unsigned int port = strtol(argv[2], NULL, 10);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Erreur lors de la création du socket\n");
        exit(1);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_aton (host, &addr.sin_addr);
    if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("Erreur lors de la connexion\n");
        exit(1);
    }
    char *line = NULL;
    size_t len = 0;
    while ((getline(&line, &len, stdin) != -1)) {
        if (strcmp(line, "exit\n") == 0) {
            printf("Fermeture de la connexion\n");
            close(sock);
            free(line);
            exit(0);
        }
        if (write(sock, line, strlen(line)) == -1) {
            perror("Erreur lors de l'écriture\n");
            exit(1);
        }
    }
    close(sock);
    free(line);
}