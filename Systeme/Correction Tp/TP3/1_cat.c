#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
 * QUESTIONS
 *
 * 1. Exécuter la commande avec un fichier texte en paramètre (le code source du
 *    programme pour l'exemple).
 *    ,----
 *    | $ ls -l 1_cat.c
 *    | -rw-r----- 1 giersch and  2006 sept. 17 15:55 1_cat.c
 *    | $ ./1_cat 1_cat.c
 *    | #include <fcntl.h>
 *    | #include <stdio.h>
 *    | [...]
 *    |     return EXIT_SUCCESS;
 *    | }
 *    | 2006 caractères du fichier 1_cat.c ont été copiés
 *    | $
 *    `----
 *
 * 2. Exécuter la commande avec un fichier binaire en paramètre, en redirigeant
 *    la sortie.
 *    On peut utiliser la commande cmp(1) pour comparer les fichiers.
 *    ,----
 *    | $ ls -l 1_cat
 *    | -rwxr-x--- 1 giersch and 13064 sept. 17 15:55 1_cat
 *    | $ ./1_cat 1_cat > 1_cat_copie
 *    | 13064 caractères du fichier 1_cat ont été copiés
 *    | $ cmp 1_cat 1_cat_copie
 *    | $ echo $?
 *    | 0
 *    | $
 *    `----
 */

#define TAILLE_TAMPON 4096

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s fichier\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *nom = argv[1];
    char tampon[TAILLE_TAMPON];
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
    return EXIT_SUCCESS;
}
