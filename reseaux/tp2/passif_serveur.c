#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "client_serveur.h"

#define SIZE 256

int main(void) {
    int cpt = 1, fils, input, nvle_connexion, socket_client, socket_serveur;
    int PORT = 3000;

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
            char message_client [SIZE];
            nvle_connexion = PORT + cpt;

            input = read(socket_client, message_client, sizeof message_client);
            if (input < 0) {
                fprintf(stderr, "Error read message from client\n");
                close(socket_client);
                return EXIT_FAILURE;
            }
            fprintf(stdout, "Message reçu : %s est le nom du fichier voulu\n", message_client);

// ==========================================================================================
            input = write(socket_client, &nvle_connexion, sizeof nvle_connexion);
            if (input != (int) sizeof nvle_connexion) {
                fprintf(stderr, "Error write server to client\n");
                close(socket_client);
                return EXIT_FAILURE;
            }
            socket_serveur = creer_serveur_tcp(nvle_connexion, 0);
            if (socket_serveur < 0) {
                fprintf(stderr, "Erreur lors de la création du serveur\n");
                return EXIT_FAILURE;
            }
            socket_client = attendre_client_tcp(socket_serveur, 0);
            if (socket_client < 0) {
                fprintf(stderr, "Echec de l'attente du client\n");
                close(socket_serveur);
                return EXIT_FAILURE;
            }
            dup2(socket_client, 1);
            char * print [] = {"cat", message_client, NULL};
            execvp(print[0], print);
            return EXIT_FAILURE;
        }
        cpt += 1;
        fprintf(stdout, "Serveur : fermeture de la socket client\n");
        close(socket_client);
    }
    return EXIT_SUCCESS;
}