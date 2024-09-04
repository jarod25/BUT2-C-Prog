#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//Serveur gérant les connexions avec des petits fils


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: port\n");
        exit(1);
    }
    unsigned int port = strtol(argv[1], NULL, 10);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Erreur lors de la création du socket\n");
        close(sock);
        exit(1);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("Erreur lors du bind\n");
        close(sock);
        exit(1);
    }
    if (listen(sock, 10) == -1) {
        perror("Erreur lors du listen\n");
        close(sock);
        exit(1);
    }
    int client_sock;
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        client_sock = accept(sock, (struct sockaddr *) &client_addr, &client_addr_len);
        if (client_sock == -1) {
            perror("Erreur lors de l'accept\n");
            continue;
        }
        char *ip_client = inet_ntoa(client_addr.sin_addr);
        int port_client = ntohs(client_addr.sin_port);
        printf("Client connecté à l'adresse %s sur le port %d\n", ip_client, port_client);
        int pid = fork();
        if (pid == -1) {
            perror("Erreur lors du fork\n");
            close(client_sock);
            continue;
        }
        if (pid == 0) {
            int pid2 = fork();
            if (pid2 == -1) {
                perror("Erreur lors du fork\n");
                close(client_sock);
                exit(1);
            }
            if (pid2 == 0) {
                char *buffer = malloc(2048);
                while (read(client_sock, buffer, sizeof buffer) != 0) {
                    printf("%s\n", buffer);
                    memset(buffer, 0, sizeof &buffer);
                }
                close(client_sock);
                free(buffer);
                exit(0);
            } else {
                close(client_sock);
                exit(0);
            }
        }
    }
}