#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "client_serveur.h"


int main(void) {
    int fils, input, socket_client, socket_serveur;
    int PORT = 4025;
    char * serveur = "127.0.0.1";

    socket_serveur = creer_serveur_tcp(PORT, 0);
    if (socket_serveur < 0) {
        fprintf(stderr, "Erreur lors de la création du serveur\n");
        return EXIT_FAILURE;
    }
    while (1) {
        socket_client = attendre_client_tcp(socket_serveur, 0);
        if (socket_client < 0) {
            fprintf(stderr, "Echec de l'attente du client\n");
            close(socket_serveur);
            return EXIT_FAILURE;
        }
        if ((fils = fork()) < 0) {
            fprintf(stderr, "Erreur fork\n");
            close(socket_client);
            close(socket_serveur);
            return EXIT_FAILURE;
        }
        if (fils == 0) {
            close(socket_serveur);

            int sizeFich;
            input = read(socket_client, &sizeFich, sizeof(int));
            if (input < 0) {
                fprintf(stderr, "Error read message from client\n");
                close(socket_client);
                return EXIT_FAILURE;
            }
            char message_client [sizeFich];
            printf("\n\t%i\n", sizeFich);

            input = read(socket_client, message_client, sizeof message_client);
            if (input < 0) {
                fprintf(stderr, "Error read message from client\n");
                close(socket_client);
                return EXIT_FAILURE;
            }
            printf("\n\t%s\n", &message_client);
            fprintf(stdout, "Message reçu : %s est le nom du fichier voulu\n", message_client);

// ==========================================================================================

            int nvle_connexion;
            input = read(socket_client, &nvle_connexion, sizeof(int));
            if (input < 0) {
                perror("Error read message from client.");
                close(socket_client);
                return EXIT_FAILURE;
            }
            fprintf(stdout, "Utilisation du canal %i pour la transmission du fichier\n", nvle_connexion);

            int socket = creer_client_tcp(serveur, nvle_connexion, 0);
            if (socket < 0) {
                fprintf(stderr, "Echec de la connexion au serveur\n");
                return EXIT_FAILURE;
            }
            dup2(socket, 1);
            char * print [] = {"cat", message_client, NULL};
            execvp(print[0], print);
            return EXIT_FAILURE;
        }
        fprintf(stdout, "Serveur : fermeture de la socket client\n");
        close(socket_client);
    }
    return EXIT_SUCCESS;
}