# include <stdio.h>

int divise(int nb) {
    if (((8*nb+1) % (3*nb+7)) == 0)
        return 0;
    return 1;
}

int main (void) {
    printf("Saisir un nombre interval (-nb, nb) : ");
    int n;
    scanf("%d", &n);
    for (int i=-n; i<n+1; i++) {
        if (divise(i) == 0)
            printf("%d\n", i);
    }
    return 0;
}