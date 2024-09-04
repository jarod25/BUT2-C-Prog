/*
 * Exemple de capture des signaux.
 *
 * Comportement "à la BSD": le gestionnaire mis en place n'est pas réinitialisé
 * lorsque le signal est traité.
 * Utilise la fonction sigaction(2).
 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* La fonction à appeler lors de la réception d'un signal */
void callback(int sig)
{
    fprintf(stderr, "> signal %d reçu: %s\n", sig, strsignal(sig));
}

int main(void)
{
    fprintf(stderr, "Mise en place du gestionnaire de signaux...\n");
    for (int i = 1; i < _NSIG; i++) {
        struct sigaction action = {
            .sa_handler = callback,
            .sa_flags   = 0,
        };
        sigemptyset(&action.sa_mask);
        if (sigaction(i, &action, NULL) == -1) {
            fprintf(stderr, "> erreur pour le signal %d \"%s\": %s\n",
                    i, strsignal(i), strerror(errno));
        }
    }

    fprintf(stderr, "En attente de signaux (pid=%d)...\n", (int)getpid());
    while (1)
        pause();

    return 0;
}
