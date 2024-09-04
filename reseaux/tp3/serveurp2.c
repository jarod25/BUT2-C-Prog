#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "client_serveur.h"

#define DEBUG 0
#define PORT 7777

int main(void) {
    int socket_client, socket_serveur, tailleIn;

    socket_serveur = creer_serveur_tcp(PORT, DEBUG);
    if (socket_serveur < 0) {
        fprintf(stderr, "Erreur lors de la création du serveur\n");
        return EXIT_FAILURE;
    }
    while (1) {
        socket_client = attendre_client_tcp(socket_serveur, DEBUG);
        if (socket_client < 0) {
            fprintf(stderr, "Echec de l'attente du client\n");
            close(socket_serveur);
            return EXIT_FAILURE;
        }
        pid_t pid = fork();
        if (pid == -1) {
            fprintf(stderr, "Echec du fork\n");
            close(socket_client);
            return EXIT_FAILURE;
        }
        if (pid == 0) {
            if (close(socket_serveur) < 0) {
                perror("Erreur close : ");
                return EXIT_FAILURE;
            }
            waitpid(socket_client, NULL, 0);
            int get1 = read(socket_client, &tailleIn, sizeof tailleIn);
            if (get1 != (int) sizeof tailleIn) {
                fprintf(stderr, "Error read size given by client\n");
                close(socket_client);
                return EXIT_FAILURE;
            }
            int *tab = malloc(sizeof(int) * tailleIn);

            int get2 = read(socket_client, tab, sizeof tab[0] * tailleIn);
            if (get2 < 0) {
                fprintf(stderr, "Error read message from client\n");
                close(socket_client);
                return EXIT_FAILURE;
            }

            printf("Message reçu : \n");
            for (int i = 0; i < tailleIn; i++) {
                printf("Message reçu n°%d: %d\n", i, tab[i]);
            }
            int j = 0;
            for (int i = 0; i < tailleIn; i++) {
                if (tab[i] != 0) {
                    tab[j] = tab[i];
                    j++;
                }
            }
            printf("Nouvelle taille : %d \n", j);
            printf("\n");

            printf("Message envoyé : \n");
            printf("\n");
            for (int i = 0; i < j; i++) {
                printf("Message envoyé n°%d: %d\n", i, tab[i]);
            }

            int put1 = write(socket_client, &j, sizeof j);
            if (put1 != (int) sizeof j) {
                fprintf(stderr, "Error write size to client\n");
                close(socket_serveur);
                return EXIT_FAILURE;
            }

            int put2 = write(socket_client, tab, sizeof tab[0] * j);
            if (put2 != (int) sizeof tab[0] * j) {
                fprintf(stderr, "Error write tab to client\n");
                close(socket_serveur);
                return EXIT_FAILURE;
            }
        free(tab);
        }
    }
}