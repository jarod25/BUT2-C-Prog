# include <stdio.h>
# include <stdlib.h>

int somme(int nb) {
    int som = 0;
    
    for (int i=1; i<=abs(nb); i++)
        som += i;
    return som;
}

int abs(int nb) {
    if (nb < 0)
        return -nb;
    return nb;
}

int main(void) {
    int nb;
    
    printf("Saisir un nombre : ");
    scanf("%d", &nb);
    
    printf("Somme de 1 à %d = %d\n", nb, somme(nb));
}