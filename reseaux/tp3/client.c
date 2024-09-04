#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "client_serveur.h"

#define DEBUG 0
#define PORT 7777

int main(void) {
    int socket_client, tailleIn, tailleOut;
    char *serveur = "localhost";

    socket_client = creer_client_tcp(serveur, PORT, DEBUG);
    if (socket_client < 0) {
        fprintf(stderr, "Failed to connect to the server\n");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Saisir la taille du tableau : \n");
    scanf("%d", &tailleIn);
    int tab [tailleIn];
    int put1 = write(socket_client, &tailleIn, sizeof tailleIn);
    if (put1 != (int) sizeof tailleIn) {
        fprintf(stderr, "Error send size to server\n");
        close(socket_client);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Saisir %d nombres pour remplir le tableau : \n", tailleIn);
    for(int i = 0; i < tailleIn; i++) {
        printf("Nombre %d: ", i);
        scanf("%d", &tab[i]);
    }

    int put2 = write(socket_client, tab, sizeof tab);
    if (put2 != (int) sizeof tab) {
        fprintf(stderr, "Error send tab to server\n");
        close(socket_client);
        return EXIT_FAILURE;
    }

    int get1 = read(socket_client, &tailleOut, sizeof tailleOut);
    if (get1 != (int) sizeof tailleOut) {
        fprintf(stderr, "Error read size from server\n");
        close(socket_client);
        return EXIT_FAILURE;
    }

    int get2 = read(socket_client, tab, sizeof(int) * tailleOut);
    if (get2 != (int) sizeof(int) * tailleOut) {
        fprintf(stderr, "Error read tab from server\n");
        close(socket_client);
        return EXIT_FAILURE;
    }
    printf("Voici les nombres triés : \n");
    for(int i = 0; i < tailleOut; i++) {
        printf("nb %d : %d \n", i, tab[i]);
    }
}