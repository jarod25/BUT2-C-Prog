# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>

#define S 60

int main(void) {
    int tab[sizeof(int)*S];
    for (int i=0; i<S; i++) {
        tab[i] = i+1;
        printf("%02d\n",tab[i]);
    }

    umask( S_IWUSR | S_IRWXG | S_IRWXO);

    FILE * resBin = fopen("res.bin", "w");
    if (resBin == NULL) {
        perror("error fopen res.bin");
        return EXIT_FAILURE;
    }
    FILE * resAscii = fopen("res.ascii", "w");
    if (resAscii == NULL) {
        perror("error fopen res.ascii");
        return EXIT_FAILURE;
    }

    FILE * resHex = fopen("res.hex", "w");
    if (resHex == NULL) {
        perror("error fopen res.hex");
        return EXIT_FAILURE;
    }

    // =========================================================
    int bin = fwrite(tab, sizeof(int), S, resBin);
    if (bin == -1) {
        perror("erreur write resBin");
        return EXIT_FAILURE;
    }
    printf("%d caractères ont été écrit dans res.bin\n", bin);
    fclose(resBin);
    
    // =========================================================
    for (int i = 0; i< S; i++)
        fprintf(resAscii, "%3d\n" , tab[i]);
    printf("%d caractères ont été écrit dans res.ascii\n", S);
    fclose(resAscii);

    // =========================================================
    for (int i = 0; i< S; i++)
            fprintf(resHex, "%08x\n", tab[i]);
    printf("%d caractères ont été écrit dans res.hex\n", S);
    fclose(resHex);

    return EXIT_SUCCESS;
}
