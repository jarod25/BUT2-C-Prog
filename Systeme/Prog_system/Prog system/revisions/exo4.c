# include <stdio.h>

void reponse(char * prenom) {
    printf("Bonjour %s\n", prenom); // str4 concatenations
}

int main(int argc, char * argv[]) {
    reponse(argv[2]);
    return 0;
}