#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* nombre de paramètres prédéfinis pour la commande xterm
 * ( 3 => "xterm" "-hold" "-e" )
 */
#define NARG 3

int main(int argc, char *argv[])
{
    char *param[NARG + argc];
    param[0] = "xterm";
    param[1] = "-hold";
    param[2] = "-e";

    /* NB: argv[argc] == NULL, par définition */
    for (int i = 1; i <= argc; i++)
        param[i + NARG - 1] = argv[i];

    /* Écriture alternative de la copie : */
    /* memcpy(param + NARG, argv + 1, argc * sizeof(char *)); */

    /* pas de paramètre ? on ne garde que la commande */
    if (argc == 1)
        param[1] = NULL;

    /*
    fprintf(stderr, "#");
    for (int i = 0; param[i] != NULL; i++)
        fprintf(stderr, " \"%s\"", param[i]);
    fprintf(stderr, "\n");
    */

    execvp(param[0], param);
    perror("execvp()");
    return EXIT_FAILURE;
}
