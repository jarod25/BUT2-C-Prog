# include <stdio.h>

int main(void) {
    int a, b, c;

    printf("Saisir a, b, c : ");
    scanf("%d %d %d", &a, &b, &c);

    int var;
    if (c > a) {
        var = a;
        a = c;
        c = var;
    }

    if (c > b) {
        var = b;
        b = c;
        c = var;
    }

    if (b > a) {
        var = a;
        a = b;
        b = var;
    }

    printf("Tri croissant : %d > %d > %d\n", a, b, c);
}