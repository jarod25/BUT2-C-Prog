# include <stdio.h>
# include <stdlib.h>

int copie(FILE * tabfish [], int argc) {
    FILE * dest = tabfish[argc-1];
    char tab[sizeof(char) * 100];
    int read, nb = 0;

    for (int i=0; i < argc-1; i++) {
        read = fread(tab, sizeof(char), 1, tabfish[i]);
        while (read != 0) {
            nb += fwrite(tab, sizeof(char), 1, dest);
            read = fread(tab, sizeof(char), 1, tabfish[i]);            
        }
    }
    return nb;
}

int main(int argc, char * argv[]) {
    if (argc == 1 || argc == 2) {
        FILE * dest = fopen("dest.txt", "w");
        for (int i=0; i < argc; i++)
            fprintf(dest, "%s", argv[i]);
        printf("Ligne de commande écrite dans le fichier de destination");
        fclose(dest);
    }

    else {
        FILE * tabfish [argc-1];
        for (int i=0; i < argc-1 && argv[1+i] != NULL; i++)
            tabfish[i] = fopen(argv[1+i], "r");

        int cp = copie(tabfish, argc);
        if (cp == -1) {
            perror("erreur copie(...)");
            return EXIT_FAILURE;
        }
        printf("%d caractères ont été copiés dans le fichier de destination.\n", cp);
        for (int i=0; i < argc-1; i++)
            fclose(tabfish[i]);
    }
    return EXIT_SUCCESS;
}