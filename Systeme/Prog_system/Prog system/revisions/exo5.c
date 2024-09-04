# include <stdio.h>

int main(void) {
    int var;
    printf("Taille du triangle de Pascal : ");
    scanf("%d", &var);

    int size = var*(var+1)/2;

    int pascal[size/2];
    pascal[0] = 1;
    for (int i=1; i<size/2; i++)
        pascal[i] = 0;


    for (int i=0; i<=var-1; i++) {
        printf("\n%d\t[%d", i, pascal[0]);
        for (int j=1; j<i; j++)
            printf(", %d", pascal[j]);
        printf(", %d]", pascal[0]);
        for (int j=i+1; j>0; j--)
            pascal[j]+=pascal[j-1];
    }
}