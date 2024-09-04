# include <stdio.h>

int abs(int nb) {
    if (nb < 0)
        return -nb;
    return nb;
}

int main(void) {
    int nb;
    printf("Calculer la valeur absolue de : ");
    scanf("%d", &nb);
    printf("Valeur absolue de %d = %d\n", nb, abs(nb));
}