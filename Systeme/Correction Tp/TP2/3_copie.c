#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

/* Fonction de copie (comme pour le premier exercice)
 * Copie le fichier `entree' dans le fichier `sortie'.
 * Retourne le nombre d'octets copiés, ou -1 en cas d'erreur.
 */
int copie(FILE *entree, FILE *sortie)
{
    int ncopied = 0;
    int c = getc(entree);
    while (c != EOF) {
        if (putc(c, sortie) == EOF)
            break;                  /* erreur: putc a échoué */
        ++ncopied;
        c = getc(entree);
    }
    if (ferror(entree) || ferror(sortie))
        return -1;
    else
        return ncopied;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr,
                "Usage: %s [source...] destination\n"
                "Copie des fichiers \"source\" dans \"destination\".\n"
                "Copie stdin si aucun fichier source n'est donné.\n",
                argv[0]);
        return 2;
    }
    const char *dest_file = argv[argc - 1];

    umask(S_IXUSR | S_IRWXG | S_IRWXO);

    // ouverture du fichier de manière exclusive (x) (glibc ou C11)
    FILE *out = fopen(dest_file, "wx");
    if (out == NULL) {
        fprintf(stderr,
                "cannot open output file \"%s\": %s\n",
                dest_file, strerror(errno));
        return 1;
    }

    int error = 0;
    if (argc == 2) {
        if (copie(stdin, out) == -1) {
            perror("erreur e/s");
            error = 1;
        }
    } else {
        for (int i = 1; i < argc - 1 && error == 0; i++) {
            const char *source_file = argv[i];
            int res = 0;
            if (strcmp(source_file, "-") == 0) {
                res = copie(stdin, out);
            } else {
                FILE *in = fopen(source_file, "r");
                if (in == NULL) {
                    fprintf(stderr,
                            "erreur d'ouverture du fichier \"%s\": %s\n",
                            source_file, strerror(errno));
                    error = 1;
                } else {
                    res = copie(in, out);
                    fclose(in);
                }
            }
            if (res == -1) {
                perror("erreur e/s");
                error = 1;
            }
        }
    }

    if (fclose(out) != 0) {
        perror("erreur en sortie");
        error = 1;
    }

    return error;
}
