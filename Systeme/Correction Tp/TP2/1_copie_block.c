#include <errno.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 32

/* Fonction de copie (par bloc)
 * Copie le fichier `entree' dans le fichier `sortie'.
 * Retourne le nombre d'octets copiés, ou -1 en cas d'erreur.
 */
int copie(FILE *entree, FILE *sortie)
{
    int ncopied = 0;
    char buf[BUFSIZE];
    size_t nread  = fread(buf, 1, sizeof buf, entree);
    while (nread > 0) {
        if (fwrite(buf, 1, nread, sortie) != nread)
            break;                  /* erreur: fwrite a échoué */
        ncopied += nread;
        nread  = fread(buf, 1, sizeof buf, entree);
    }
    if (ferror(entree) || ferror(sortie))
        return -1;
    else
        return ncopied;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr,
                "Usage: %s entree sortie\n"
                "Copie du fichier \"entree\" dans le fichier \"sortie\".\n",
                argv[0]);
        return 2;
    }
    const char *input_file = argv[1];
    const char *output_file = argv[2];

    FILE *in = fopen(input_file, "r");
    if (in == NULL) {
        fprintf(stderr,
                "erreur à l'ouverture du fichier source \"%s\": %s\n",
                input_file, strerror(errno));
        return 1;
    }
    FILE *out = fopen(argv[2], "w");
    if (out == NULL) {
        fprintf(stderr,
                "erreur à l'ouverture du fichier destination \"%s\": %s\n",
                output_file, strerror(errno));
        fclose(in);
        return 1;
    }

    int n = copie(in, out);
    int error = 0;
    if (n >= 0) {
        fprintf(stderr, "Copie de %d octet%s\n", n, (n > 1 ? "s" : ""));
    } else {
        perror("erreur e/s");
        error = 1;
    }

    if (fclose(out) != 0) {
        perror("erreur en sortie");
        error = 1;
    }
    fclose(in);              /* inutile ici de vérifier le retour de fclose() */

    return error;
}
