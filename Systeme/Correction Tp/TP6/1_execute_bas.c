/*
 * Redirections, bas niveau
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
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
    if (entree) {
        int fd = open(entree, O_RDONLY);
        if (fd == -1)
            die("open(input)");
        if (dup2(fd, STDIN_FILENO) == -1)
            die("dup(stdin)");
        close(fd);
    }
    if (sortie) {
        int fd = creat(sortie, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (fd == -1)
            die("creat(output)");
        if (dup2(fd, STDOUT_FILENO) == -1)
            die("dup(stdout)");
        close(fd);
    }
    if (erreur) {
        int fd = creat(erreur, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (fd == -1)
            die("creat(error)");
        if (dup2(fd, STDERR_FILENO) == -1)
            die("dup(stderr)");
        close(fd);
    }

    /* Exécution de la commande */
    execvp(args[0], args);
    die("execvp");
    return 0;
}
