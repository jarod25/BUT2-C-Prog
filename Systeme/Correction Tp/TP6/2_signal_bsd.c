/*
 * Exemple de capture des signaux.
 *
 * Comportement "à la BSD": le gestionnaire mis en place n'est pas réinitialisé
 * lorsque le signal est traité.
 * Utilise la fonction signal(2).
 */

/* Permet de sélectionner l'implémentation de signal(2) à utiliser */
#define _DEFAULT_SOURCE

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
        if (signal(i, callback) == SIG_ERR) {
            fprintf(stderr, "> erreur pour le signal %d \"%s\": %s\n",
                    i, strsignal(i), strerror(errno));
        }
    }

    fprintf(stderr, "En attente de signaux (pid=%d)...\n", (int)getpid());
    while (1)
        pause();

    return 0;
}
