/*
 * Redirections, haut niveau
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void die(const char *perror_msg)
{
    if (perror_msg)
        perror(perror_msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int mauvaise_option = 0;    /* 1 si erreur dans les options */
    const char *entree = NULL;  /* fichier pour rediriger stdin */
    const char *sortie = NULL;  /* fichier pour rediriger stdout */
    const char *erreur = NULL;  /* fichier pour rediriger stderr */

    /* Lecture des options */
    int opt;
    while ((opt = getopt(argc, argv, "i:o:e:")) != -1) {
        switch (opt) {
        case 'i':
            entree = optarg;
            break;
        case 'o':
            sortie = optarg;
            break;
        case 'e':
            erreur = optarg;
            break;
        default:
            mauvaise_option = 1;
            break;
        }
    }
    char **args = argv + optind;

    /* Vérification des arguments */
    if (args[0] == NULL) {
        fprintf(stderr,
                "Usage:\n"
                "  %s [-i entrée] [-o sortie] [-e erreur] [--] commande [args...]\n",
                argv[0]);
        die(NULL);
    }
    if (mauvaise_option)
        die(NULL);

    /* Mise en place des redirections */
    if (entree && freopen(entree, "r", stdin) == NULL)
        die("freopen(stdin)");
    if (sortie && freopen(sortie, "w", stdout) == NULL)
        die("freopen(stdout)");
    if (erreur && freopen(erreur, "w", stderr) == NULL)
        die("freopen(stderr)");

    /* Exécution de la commande */
    execvp(args[0], args);
    die("execvp");
    return 0;
}
