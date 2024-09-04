#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define N 60

int main() {

    int tab[N];

    for (int i = 0; i < N; ++i) {
        tab[i]=i+1;
        printf("%02i\n", tab[i]);
    }

    umask(S_IWUSR | S_IRWXG | S_IRWXO );

    FILE * outBi = fopen("res.bin", "w");
    FILE * outAsc = fopen("res.ascii", "w");
    FILE * outHex = fopen("res.hex", "w");
    if (outBi == NULL || outAsc == NULL || outHex == NULL){
        fprintf(stderr, "The file can't be openned !\n");
        return EXIT_FAILURE;
    }
    /*
    for (int j = 0; j < N; ++j)
        printf("tab[%u] = %d\n", j, tab[j]);

    for (int k = 0; k < N; ++k) {
        fputc(tab[k], outBi);
    }
    for (int l = 0; l < N; ++l) {
        (tab[l]%2)/16;
        fputc(tab[l], outAsc);
    }
    for (int m = 0; m < N; ++m) {
        (tab[m]%2)/2;
        fputc(tab[m], outHex);
    }
     */

    int nb = fwrite(tab, sizeof(int), N, outBi);
    if (nb == 61) {
        perror("Erreur écriture res.bin");
        return EXIT_FAILURE;
    }

    for (int i=0; i<N; i++) {
        fprintf(outAsc, "%3d\n", tab[i]);
    }

    for (int i = 0; i < N; ++i) {
        fprintf(outHex, "%08X\n", tab[i]);
    }
    return EXIT_SUCCESS;
}