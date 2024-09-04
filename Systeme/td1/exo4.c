#include <stdio.h>

/*int main(int argc, char *argv[])
{
    char prenom[100];

    printf("Comment t'appelles-tu ? \n");
    scanf("%s", prenom);
    printf("Bonjour %s !\n", prenom);

    return 0;
}*/

/*char carac() {
    char * prenom;
    scanf("%s", prenom);
    return prenom;
}

int main(int argc, char *argv[]){

    printf("Comment t'appelles-tu ? \n");
    printf("Salut %s, je suis heureux de te rencontrer !\n", carac());

    return 0;
}*/


int main(int argc, char * argv[]) {
    printf("Bonjour %s\n", argv[1]);
    return 0;
}
