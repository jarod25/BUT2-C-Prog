#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define TAILLE_TAMPON 4096
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s fichier\n", argv[0]);
        return EXIT_FAILURE;
    }

    char tampon[TAILLE_TAMPON];
    if (argc == 2) {
        printf("bippppppppp");
        const char *nom = argv[1];
        int fich, nlus, necrits;
        int total = 0;
        fich = open(nom, O_RDONLY);
        if (fich < 0) {
            perror("Ouverture du fichier");
            return EXIT_FAILURE;
        }
        do {
            nlus = read(fich, tampon, TAILLE_TAMPON);
            if (nlus < 0) {
                perror("Erreur de lecture");
                close(fich);
                return EXIT_FAILURE;
            }
            total += nlus;
            necrits = write(1, tampon, nlus);
            if (necrits != nlus)
                fprintf(stderr, "%d caractères écrits au lieu de %d lus\n",
                        necrits, nlus);
        } while (nlus != 0);
        fprintf(stderr, "%d caractères du fichier %s ont été copiés\n",
                total, nom);
        close(fich);
    }

    else {
        int fich [argc-2];
        int fichOut = open(argv[argc-1], O_WRONLY);

        int nread, nwrite, total;
        for (int i=0; i < argc-2 && argv[i+1] != NULL; i++)
            fich [i] = open(argv[i+1], O_RDONLY);

        for (int i=0; i<argc-2; i++) {
             do {
                nread = read(fich[i], tampon, 1);
                if (nread < 0) {
                    perror("Erreur de lecture");
                    if (tabClose(argc-2, fich) == EXIT_FAILURE)
                        perror("Erreur fermeture fichiers.");
                    close(fichOut);
                    return EXIT_FAILURE;
                }
                nwrite = write(fichOut, tampon, nread);
                total += nwrite;
                if (nwrite != nread)
                    fprintf(stderr, "%d caractères écrits au lieu de %d", nwrite, nread);
            } while (nread != 0);
        }

        if (tabClose(argc-2, fich) == EXIT_FAILURE || close(fichOut) != 0) {
            perror("Erreur fermeture fichiers.");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int tabClose(int argc, int tab []) {
    for (int i=0; i<argc; i++) {
        if (close(tab[i]) != 0) {
            fprintf(stderr, "Erreur de fermeture pour la position %d.\n", i);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

// lrsq on redirige la sortie d'un fichier binaire dans un autre fichier, les 2 fichiers ont la m^ taille