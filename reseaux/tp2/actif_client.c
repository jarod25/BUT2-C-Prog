#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "client_serveur.h"

#define SIZE 256
#define PORT 4025

int main (void) {
    int put, socket;
    char message_serveur [SIZE];
    char * serveur = "127.0.0.1";
    fprintf(stderr, "Connexion au serveur : %s au port : %d\n", serveur, PORT);

    socket = creer_client_tcp(serveur, PORT, 0);
    if (socket < 0) {
        fprintf(stderr, "Echec de la connexion au serveur\n");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Saisir le nb de caractère du fichier : ");
    int sizeFich;
    if (scanf("%i", &sizeFich) < 0) {
        fprintf(stderr, "Error scanf\n");
        close(socket);
        return EXIT_FAILURE;
    }
    sizeFich ++;
    put = write(socket, &sizeFich, sizeof(int));
    if (put != (int) sizeof(int)) {
        fprintf(stderr, "Error write client to server\n");
        close(socket);
        return EXIT_FAILURE;
    }
    char fich [sizeFich];

    fprintf(stdout, "\nSaisir le nom du fichier désiré : ");
    if (scanf("%s", fich) < 0) {
        fprintf(stderr, "Error scanf\n");
        close(socket);
        return EXIT_FAILURE;
    }
    put = write(socket, fich, sizeof fich);
    if (put != (int) sizeof fich) {
        fprintf(stderr, "Error write client to server\n");
        close(socket);
        return EXIT_FAILURE;
    }
// ==========================================================================================
    int newPort = 4000 + rand() % 10;
    fprintf(stdout, "\n\tPort utilisé pour la transaction : %i\n", newPort);

    put = write(socket, &newPort, sizeof(int));
    if (put != (int)sizeof(int)) {
        fprintf(stderr, "Error write server to client\n");
        close(socket);
        return EXIT_FAILURE;
    }

    socket = creer_serveur_tcp(newPort, 0);
    if (socket < 0) {
        fprintf(stderr, "Erreur lors de la création du serveur\n");
        return EXIT_FAILURE;
    }
    int socket_client = attendre_client_tcp(socket, 0);
    if (socket_client < 0) {
        fprintf(stderr, "Echec de l'attente du client\n");
        close(socket);
        return EXIT_FAILURE;
    }
    put = read(socket_client, message_serveur, sizeof message_serveur);
    if (put < 0) {
        fprintf(stderr, "Error read message from server\n");
        close(socket);
        close(socket_client);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Contenu du fichier %s :\n%s\n", fich, message_serveur);
    return EXIT_SUCCESS;
}