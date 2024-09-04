#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SIZE 100

int copieFile(int src, int dest) {
    int nlus, necrit, total = 0;
    char tab [SIZE];
    do {
        nlus = read(src, tab, 1);
        if (nlus < 0) {
            perror("Erreur lecture");
            return EXIT_FAILURE;
        }
        necrit = write(dest, tab, nlus);
        if (necrit < 0) {
            perror("Erreur écriture");
            return EXIT_FAILURE;
        }
        total += necrit;
        if (nlus > necrit)
            fprintf(stderr, "%d caractères écrits --- %d lus\n", necrit, nlus);
    } while (nlus != 0);
    return EXIT_SUCCESS;
}

int main (int argc, char * argv[]) {
    if (argc < 3) {
        fprintf(stderr, "2 fichiers attendus\n");
        return EXIT_FAILURE;
    }

    int src = open(argv[1], O_RDONLY);
    int dest = open(argv[2], O_CREAT | O_EXCL | O_WRONLY, S_IRWXU);

    if (src < 0 || dest < 0) {
        perror("Erreur ouverture\n");
        return EXIT_FAILURE;
    }

    if (copieFile(src, dest) == EXIT_FAILURE) {
        perror("Erreur lors de la copie\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}