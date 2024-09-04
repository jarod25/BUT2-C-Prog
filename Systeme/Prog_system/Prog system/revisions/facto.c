# include <stdio.h>

int facto(int nb) {
    if (nb == 1)
        return 1;
    return nb*facto(nb-1);
}

int main(void) {
    int nb;
    printf("Calcul de la factorielle de : ");
    scanf("%d", &nb);

    printf("Factorielle de %d = %d\n", nb, facto(nb));
}