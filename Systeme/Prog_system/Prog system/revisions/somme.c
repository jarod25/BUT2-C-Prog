# include <stdio.h>

int som(int a, int b) {
    return a+b;
}

int somme(int nb) {
    return nb*(nb+1)/2;
}

int sommeBoucle(int nb) {
    int som = 1;
    for (int i=2; i<nb+1; i++)
        som = som +i;
    return som;
}

int main(void) {
    int a, b;
    printf("Saisir nb a et b à sommer : ");
    scanf("%d%d", &a, &b);
    printf("Somme de %d et %d = %d.\n", a, b, som(a, b));

    printf("Faire la somme de 1 à : ");
    scanf("%d", &a);
    printf("Somme sans boucle de 1 à %d = %d.\n", a, somme(a));
    printf("Somme avec boucle de 1 à %d = %d.\n", a, somme(a));
}