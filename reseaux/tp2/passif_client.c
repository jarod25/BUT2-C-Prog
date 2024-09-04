#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "client_serveur.h"

#define SIZE 256
#define PORT 3000

int main (void) {
    int input, output, socket;
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
    char fich [sizeFich];

    fprintf(stdout, "\nSaisir le nom du fichier désiré : ");
    if (scanf("%s", fich) < 0) {
        fprintf(stderr, "Error scanf\n");
        close(socket);
        return EXIT_FAILURE;
    }
    output = write(socket, fich, sizeof fich);
    if (output != (int) sizeof fich) {
        fprintf(stderr, "Error write client to server\n");
        close(socket);
        return EXIT_FAILURE;
    }
// ==========================================================================================
    int newPort;
    input = read(socket, &newPort, sizeof(newPort));
    if (input < 0) {
        fprintf(stderr, "Error read message from server\n");
        close(socket);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Utilisation du canal %i pour la transmission du fichier\n", newPort);
    socket = creer_client_tcp(serveur, newPort, 0);
    if (socket < 0) {
        fprintf(stderr, "Echec de la connexion au serveur\n");
        exit(EXIT_FAILURE);
    }
    input = read(socket, message_serveur, sizeof message_serveur);
    if (input < 0) {
        fprintf(stderr, "Error read message from server\n");
        close(socket);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Contenu du fichier %s :\n%s\n", fich, message_serveur);
        
    return EXIT_SUCCESS;
}