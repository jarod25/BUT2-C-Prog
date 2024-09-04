#include <stdio.h>

void main() {
    int nbr, i, j, p=1;

    printf("Entrez n: ");
    scanf("%d",&nbr);

    for(i = 0; i < nbr; i++) {
        for(j = 0; j <= i; j++) {
            if ( i==0 || j==0 )
                p = 1;
            else
                p = p*(i-j+1)/j;
            printf("% 4d",p);
        }
        printf("\n");
    }
}