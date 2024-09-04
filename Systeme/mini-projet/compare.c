#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

/**
 * @brief Comparaison de programmes
 *
 * @param argc donne le nombre d'éléments de la ligne de commande
 * @param argv contient ces éléments sous la forme d'un tableau de chaînes de caractères
 * @return int
 */
int main(int argc, char *argv[]) {
    // Initialisation des varibles
    int status;
    int save_out = dup(fileno(stdout));
    char * cmd1[argc];
    char * cmd2[argc];
    int cpt = 0;

    // Récupère les arguments de la commande 1 et les ajouter dans le tableau cmd1
    for (int i = 0; i < argc; i++){

        /* Permet de comparer 2 chaînes de caractères
         * et de savoir si la première est égale à la seconde
         */
        if (strcmp(argv[i+1],"--") == 0){
            cmd1[i+1] = NULL;
            break; // Lorsque la chaîne de caractère est égal à "--", on sort de la boucle for
        }
        cmd1[i] = argv[i+1];
        cpt++;
    }

    // Récupère les arguments de la commande 2 et les ajouter dans le tableau cmd2
    int j = 0; // Initialise une variable pour la position du tableau cmd2
    for (int i = cpt+1; i < argc-1; i++){
        if (i+1 ==  argc){
            cmd2[i+1] = NULL;
        }
        cmd2[j] = argv[i+1];
        j++;
    }

    // On utilise un fork afin d'exécuter la commande 1 et continuer
    pid_t exec1 = fork();
    if (exec1 == -1) {
        perror("fork1");
        return EXIT_FAILURE;
    }
    if (exec1 == 0) {
        // Créer ou écraser s'il existe output1.txt et l'ouvrir en écriture seule
        int output1 = open("output1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(output1, fileno(stdout));
        execvp(cmd1[0], cmd1); // Exécution de la commande 1
        close(output1);
    }
    waitpid(exec1, &status, 0); // Attend que le processus exec1 se termine

    // On utilise un fork afin d'exécuter la commande 2 et continuer
    pid_t exec2 = fork();
    if (exec2 == -1) {
        perror("fork2");
        return EXIT_FAILURE;
    }
    if (exec2 == 0) {
        // Créer ou écraser s'il existe output2.txt et l'ouvrir en écriture seule
        int output2 = open("output2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(output2, fileno(stdout));
        execvp(cmd2[0], cmd2); // Exécution de la commande 2
        close(output2);
    }
    waitpid(exec2, &status, 0); // Attend que le processus exec2 se termine

    // On utilise un fork afin d'exécuter la commande diff -u et continuer
    pid_t exec3 = fork();
    if (exec3 == -1) {
        perror("fork3");
        return EXIT_FAILURE;
    }
    if (exec3 == 0) {
        dup2(save_out, fileno(stdout)); // Permet de retourner sur stdout
        execlp("diff", "diff", "-u", "output1.txt", "output2.txt", NULL); // Exécution de la commande diff -u
    }
    else {
        waitpid(exec3, &status, 0); // Attend que le processus exec3 se termine
        if (WIFEXITED(status)) { // Compare les 2 fichiers
            if (WEXITSTATUS(status) == 0) {
                printf("Les fichiers sont identiques\n");
            } else {
                printf("Les fichiers sont différents\n");
            }
        }

        // Supprimer les fichiers output1.txt et output2.txt
        remove("output1.txt");
        remove("output2.txt");
        return 0;
    }
    return 0;
}
