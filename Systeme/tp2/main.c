#include <stdio.h>

int copie(FILE *entree, FILE *sortie) {

    int n = 0;
    char ch;
    ch = fgetc(entree);
    while (ch != EOF) {
        fputc(ch, sortie);
        n++;
        ch = fgetc(entree);
    }
    return n;
}

int main() {
    char entree[100];
    char sortie[100];
    printf("Enter source file path: ");
    scanf("%s", entree);
    printf("Enter destination file path: ");
    scanf("%s", sortie);
    FILE * in = fopen(entree, "r");
    FILE * out = fopen(sortie, "w");
    if (in == NULL || out == NULL){
        fprintf(stderr, "One of the file don't exist !\n");
        return -1;
    }
    int n = copie(in, out);
    printf("Copie a retourné : %d\n", n);
    if (fclose(out) != 0) {
        perror("fermeture(entree)");
        return -1;
    }
    fclose(in);
    printf("%s", "file copied \n");
}