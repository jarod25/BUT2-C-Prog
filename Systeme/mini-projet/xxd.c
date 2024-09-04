#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 4096 // taille maximum du buffer

/**
 * Affiche le contenu d'un fichier ou d'un texte en hexadécimal
 *
 * @param offset : décalage du début de l'affichage
 * @param addr : adresse de début de l'affichage
 * @param len : longueur de l'affichage
 */
void writeLine(int offset, void *addr, int len) {
    int i;
    char line[17];
    unsigned char *car = addr;

    for (i = 0; i < len; i++) {
        if ((i % 16) == 0) { // si on est au début d'une ligne
            if (i != 0) // si on est à la fin de la ligne
                printf(" %s\n", line);
            printf("%08x: ", offset);
            offset += (i % 16 == 0) ? 16 : i % 16; // on incrémente l'offset
        }
        printf("%02x", car[i]);
        if ((i % 2) == 1) // si on est au milieu d'un octet
            printf(" ");

        if (isprint(car[i])) { // si le caractère est imprimable
            line[i % 16] = car[i];
        }
        else {
            line[i % 16] = '.';
        }
        line[(i % 16) + 1] = '\0';
    }

    while ((i % 16) != 0) { // si on est pas à la fin d'une ligne
        printf("  ");
        if (i % 2 == 1) // si on est au milieu d'un octet
            putchar(' ');
        i++;
    }
    printf(" %s\n", line);
}

/**
 * @brief
 *
 * @param argc : nombre d'arguments
 * @param argv : tableau d'arguments
 * @return int : code de retour
 */
int main(int argc, char *argv[]) {
    if (argc == 2) {
        FILE *FILE = fopen(argv[1], "rb");
        if (FILE == 0) { // si le fichier n'existe pas
            fprintf(stderr, "%s: failed to open file '%s' for reading\n", argv[0], argv[1]);
            exit(EXIT_FAILURE);
        }
        char buffer[SIZE];
        int n = fread(buffer, 1, SIZE, FILE);
        if (n > SIZE) { // si le fichier est trop grand
            fprintf(stderr, "%s: file '%s' is too large, maximum %d character\n", argv[0], argv[1], SIZE);
            exit(EXIT_FAILURE);
        }
        if (n == 0) { // si le fichier est vide
            fprintf(stderr, "%s: failed to read file '%s'\n", argv[0], argv[1]);
            exit(EXIT_FAILURE);
        }
        int offset = 0;
        writeLine(offset, buffer, n);
        fclose(FILE);
    }
    if (argc == 1) {
        char buffer[SIZE];
        int i = scanf("%[^\n]", buffer);
        if (i == 0) { // si l'entrée est vide
            fprintf(stderr, "%s: failed to read input\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        int n = strlen(buffer);
        if (n > SIZE) { // si le texte est trop grand
            fprintf(stderr, "%s: text is too large, maximum %d character\n", argv[0], SIZE);
            exit(EXIT_FAILURE);
        }
        int offset = 0;
        writeLine(offset, buffer, n);
    }
    return 0;
}

