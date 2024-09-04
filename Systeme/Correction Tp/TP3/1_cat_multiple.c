#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define TAILLE_TAMPON 4096

int cat(const char *nom)
{
    char tampon[TAILLE_TAMPON];
    int fich, nlus, necrits;
    int total = 0;
    fich = open(nom, O_RDONLY);
    if (fich < 0) {
        perror("Ouverture du fichier");
        return -1;
    }
    do {
        nlus = read(fich, tampon, TAILLE_TAMPON);
        if (nlus < 0) {
            perror("Erreur de lecture");
            close(fich);
            return -1;
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
    return total;
}

int main(int argc, char *argv[])
{
    int total = 0;
    int res = 0;
    for (int i = 1; res >= 0 && i < argc; i++) {
        res = cat(argv[i]);
        if (res >= 0)
            total += res;
    }
    fprintf(stderr, "TOTAL: %d caractères de %d fichier(s) ont été copiés\n",
            total, argc - 1);
    return res >= 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
