#include <stdio.h>

int main(){
    int i, n, s = 0;

    printf("Donner un entier n : ");
    scanf("%d",&n);

    for(i=1;i<=n;i++) {
        s = s  + i;
    }
    printf("\nLa somme des %d premiers entiers est : %d\n",n, s);
}