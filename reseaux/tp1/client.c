#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "client_serveur.h"

#define SIZE 256
#define PORT 3000

int main(void) {
    int input, output, socket;

    char message_client[SIZE];
    char message_serveur[SIZE];
    char * serveur = "localhost";

    fprintf(stderr, "Connected to %s on port %d\n", serveur, PORT);
    fprintf(stdout, "Veuillez écrire un entier : \n");

    socket = creer_client_tcp(serveur, PORT, 0);
    if (socket < 0) {
        fprintf(stderr, "Fail to connect to server\n");
        exit(EXIT_FAILURE);
    }


    input = scanf("%s", message_client);
    if (input < 0) {
        fprintf(stderr, "Fail to read input\n");
        close(socket);
        exit(EXIT_FAILURE);
    }

    output = write(socket, message_client, sizeof message_client);
    if (output != sizeof message_client) {
        fprintf(stderr, "Fail to send message to server\n");
        exit(EXIT_FAILURE);
    }

    input = read(socket, message_serveur, sizeof message_serveur);
    if (input <= 0) {
        fprintf(stderr, "Fail to receive message from server\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "%s\n", message_serveur);

    int nb = atoi(message_client), cpt = 0;

    while (cpt < nb) {
        input = scanf("%s", message_client);
        if (input < 0) {
            fprintf(stderr, "Fail to read input\n");
            close(socket);
            exit(EXIT_FAILURE);
        }

        output = write(socket, message_client, input);
        if (output != input) {
            fprintf(stderr, "Fail to send message to server\n");
            exit(EXIT_FAILURE);
        }
        cpt += 1;
    }

    input = read(socket, message_serveur, sizeof message_serveur);
    if (input <= 0) {
        fprintf(stderr, "Error during arguments reading\n\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "%s\n", message_serveur);

    close(socket);
    exit(EXIT_SUCCESS);
}