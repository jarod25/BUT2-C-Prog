#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if __GNUC__
#define unused __attribute__((unused))
#endif

void pruid(const char *msg)
{
    fprintf(stderr, "# %s: real UID=%-8d effective UID=%d\n",
            msg, (int)getuid(), (int)geteuid());
}

int main(unused int argc, char *argv[])
{
    if (getuid() == geteuid()) {
        fprintf(stderr,
"Usage: cette commande doit être exécutée par un utilisateur non privilégié.\n"
"   Pour pouvoir l'utiliser comme demandé, il faut d'abord, (1) changer le\n"
"   propriétaire du fichier puis, (2) positionner le bit de prise d'identité :\n"
"        chown root %s\n"
"        chmod u+s %s\n",
                argv[0], argv[0]);
        return EXIT_FAILURE;
    }

    pruid("AVANT");
    if (1) {
        /* Change l'UID réel pour qu'il soit égal à l'UID effectif.  Cette
         * manipulation n'est normalement pas nécessaire.
         *
         * Elle l'est pour cet exercice car la commande lancée (xterm) commence
         * par réinitialiser l'UID effectif à l'UID réel...
         */
        setuid(geteuid());
        pruid("APRES");
    }

    execlp("xterm", "xterm", (char*)NULL);
    perror("execlp()");
    return EXIT_FAILURE;
}
