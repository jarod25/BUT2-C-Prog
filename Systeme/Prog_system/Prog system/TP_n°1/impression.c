# include <stdio.h>
# include <stdlib.h>

int copie(FILE * entree, FILE * sortie) {
    char tab [sizeof(char) * 100];
    int nb = 0, var = fread(tab, sizeof(char), 1, entree);;
    while (var > 0) {
        nb += fwrite(tab, sizeof(char), 1, sortie);
        var = fread(tab, sizeof(char), 1, entree);
    }

    // char c = fgetc(entree);
    // while (c != EOF) {
    //     fputc(c, sortie);
    //     c = fgetc(entree);
    //     nb ++ ;
    // }

    if (ferror(entree) || ferror(sortie))
        return -1;
    return nb;
}

int main(void) {
    FILE * origin = fopen("origine.txt", "r");
    FILE * dest = fopen("dest.txt", "w");
    if (origin == NULL || dest == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    printf("Copie de %d éléments\n", copie(origin, dest));
    if (fclose(dest) != 0) {
        fprintf(stderr, "Erreur fclose de destination");
        return EXIT_FAILURE;
    }
    fclose(origin);
    return EXIT_SUCCESS;
}
