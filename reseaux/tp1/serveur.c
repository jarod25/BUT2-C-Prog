#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "client_serveur.h"

#define SIZE 256
#define PORT 3000

void sigchld_handler(int sig) {
    while (waitpid(-1, 0, WNOHANG) > 0){
        printf("Child %d process terminated\n", sig);
    }
}


int main(void) {
    int response, socket_client, socket_server, fils;
    fprintf(stderr, "Server connected on port %d\n", PORT);
    socket_server = creer_serveur_tcp(PORT, 0);
    if (socket_server < 0) {
        fprintf(stderr, "Fail to create server \n");
        exit(EXIT_FAILURE);
    }
    while (1) {
        socket_client = attendre_client_tcp(socket_server, 0);
        if (socket_client < 0) {
            fprintf(stderr, "Connection failed with the client\n");
            exit(EXIT_FAILURE);
        }
        if ((fils = fork()) < 0) {
            fprintf(stderr, "Erreur fork\n");
            close(socket_client);
            close(socket_server);
            return EXIT_FAILURE;
        }
        if (fils == 0) {
            close(socket_server);
            char message_client[SIZE];
            char message_server[SIZE];
            int cpt = 0, sizeTab;
            float som = 0;


            response = read(socket_client, message_client, sizeof message_client);
            if (response <= 0) {
                fprintf(stderr, "No message receive\n");
                close(socket_client);
                exit(EXIT_FAILURE);
            }
            fprintf(stdout, "taille du tableau = %s\n", message_client);
            sizeTab = atoi(message_client);
            double tab[sizeTab];

            sprintf(message_server, "Veuillez écrire %i nombres :", sizeTab);

            response = write(socket_client, message_server, sizeof message_server);
            if (response != sizeof message_server) {
                fprintf(stderr, "Fail during sending arguments \n");
                close(socket_client);
                exit(EXIT_FAILURE);
            }

            while (cpt < sizeTab) {
                response = read(socket_client, message_client, sizeof(double));
                tab[cpt] = atof(message_client);
                som += tab[cpt];
                cpt += 1;
            }
            fprintf(stdout, "Somme = %f\n", som);
            sprintf(message_server, "Somme = %f", som);

            response = write(socket_client, message_server, sizeof message_server);
            if (response != sizeof message_server) {
                fprintf(stderr, "Fail to send message to the client\n");
                close(socket_client);
                exit(EXIT_FAILURE);
            }
            signal(SIGCHLD, sigchld_handler);
        }
        fprintf(stderr, "Server close client's socket\n");
        close (socket_client);
        raise(SIGUSR1);
        signal(SIGUSR1,kill);
    }
}