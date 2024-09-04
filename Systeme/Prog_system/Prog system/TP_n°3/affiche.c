# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>



void affiche_car(const char * prefixe) {
    printf("%s Processus n°%d. N° du processus père : %d\n.", prefixe, getpid(), getppid());
}

int fils(const char * var) {
    int fils = fork();
    switch (fils) {
        case -1:
            perror("fork");
            return EXIT_FAILURE;
        case 0:
            sleep(5);
            // affiche_car(var);
            break;
        default:
            affiche_car(var);
            break;
    }
    return EXIT_SUCCESS;
}

int famille(const char * var) {
    pid_t aine, cadet, petit_fils;
    aine = fork();
    switch (aine) {
        case -1:
            perror("fork()");
            return EXIT_FAILURE;
        case 0:
            petit_fils = fork();
            break;
        default:
            cadet = fork();
            if (cadet == 0)
                petit_fils = fork();
            break;
    }
    // wait();
    // WIFEXITED()
    affiche_car(var);
    return 0;
}

int main(void) {
    int size;
    printf("Longueur de la chaîne : ");
    if (scanf("%i", &size) == -1) {
        perror("Erreur scan size tab");
        return EXIT_FAILURE;
    }
    char var [size];
    printf("Chaine de caractère : ");
    if (scanf("%s", var) == -1) {
        perror("Erreur scan chaine de caractère");
        return EXIT_FAILURE;
    }

    // if (fils(var) == EXIT_FAILURE)
    //     return EXIT_FAILURE;
    
    famille(var);
    return EXIT_SUCCESS;
}
