#include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void) {
    int tab [100];
    int fich = open("tab.bin", O_WRONLY | O_TRUNC);
    for (int i=0; i<100; i++)
        tab[i] = i;
    int nwrite = write(fich, tab, 100);
    
    if (nwrite < 0) {
        perror("Erreur d'écriture");
        return EXIT_FAILURE;
    }

    if (nwrite != 100)
        fprintf(stderr, "%d caractères écrits au lieu de 100", nwrite);
    
    printf("%d caractères écrits dans tab.bin.\n", nwrite);
    return EXIT_SUCCESS;
}