# include <stdio.h>

int v(int nb) {
    if (nb == 0)
        return 1745;
    return v(nb-1)*0.9972+2123;
}

int main(void) {
    int nb;
    printf("Saisir n : ");
    scanf("%d", &nb);

    printf("v(%d) = %d\n", nb, v(nb));
}