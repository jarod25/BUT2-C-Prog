#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
# include <string.h>

int main (int argc, char * argv []) {
    
    // ========================================== haut niveau
    // FILE * file = freopen("entree.txt", "w+", stdin);
    // if (strcmp(argv[1], "-o") == 0)
    //     file = freopen("sortie.txt", "w+", stdout);
    // else if (strcmp(argv[1], "-e") == 0)
    //     file = freopen("erreur.txt", "w+", stderr);

    // ========================================== bas niveau
    int fd = open("entree.txt", O_WRONLY);
    int bn = dup2(fd, STDOUT_FILENO);
    close(fd);

    char * tab[argc-1];
    for (int i=0; i<argc-2; i++)
        tab[i] = argv[i+2];
    tab[argc-2] = NULL;

    if (execv("/usr/bin/cat", tab) == -1) {
        perror("Error excev");
        return EXIT_FAILURE;
    }
    // fclose(file);
    close(bn);
    return EXIT_SUCCESS;
}