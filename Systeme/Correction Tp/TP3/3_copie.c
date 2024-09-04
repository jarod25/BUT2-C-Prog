#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    int force = 0;              /* écrase le fichier destination si 1 */
    int append = 0;             /* ouverture en mode ajout */
    int argn;                   /* position du paramètre `source' dans argv[] */

    /* Lecture des options */
    for (argn = 1; argn < argc; argn++) {
        if (strcmp(argv[argn], "-a") == 0)
            append = 1;
        else if (strcmp(argv[argn], "-f") == 0)
            force = 1;
        else
            break;
    }
    if (argn != argc - 2) {
        fprintf(stderr, "Usage: %s [-a|-f] source destination\n", argv[0]);
        exit(1);
    }
    const char *inf = argv[argn]; /* fichier source */
    const char *outf = argv[argn + 1]; /* fichier destination */

    mode_t omode = S_IRUSR | S_IWUSR;
    int oflags = O_WRONLY | O_CREAT;
    if (append) {
        /* Si l'option `append' a été spécifiée, on ajoute le flag O_APPEND pour
           que l'ouverture se fasse en mode ajout. */
        oflags |= O_APPEND;
    } else {
        if (force) {
            /* Sinon, si l'option `force' a été spécifiée, on ajoute le flag
               O_TRUNC pour tronquer le fichier destination. */
            oflags |= O_TRUNC;
        } else {
            /* Sinon, on ajoute le flag O_EXCL pour que l'ouverture échoue si le
               fichier destination existe déjà. */
            oflags |= O_EXCL;
        }
    }

    int fd0, fd1;
    if ((fd0 = open(inf, O_RDONLY)) == -1) {
        perror("open(inf)");
        exit(2);
    }
    if ((fd1 = open(outf, oflags, omode)) == -1) {
        perror("open(outf)");
        exit(3);
    }

    char buffer[4096];
    ssize_t rd;
    do {
        rd = read(fd0, buffer, sizeof buffer);
        if (rd == -1) {
            perror("read()");
            exit(2);
        }
        if (write(fd1, buffer, rd) != rd) {
            perror("write()");
            exit(3);
        }
    } while (rd > 0);

    if (close(fd1) == -1) {
        perror("close(outf)");
        exit(3);
    }
    close(fd0);
    return 0;
}
