/*
 * Affichage d'un message en boucle en utilisant plusieurs processus
 * synchronisés par signaux.
 *
 * Usage: ./3_synchro [-debug]
 */

#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <sys/wait.h>

#if __GNUC__
#define unused __attribute__((unused))
void pr_debug(const char *format, ...) __attribute__((format (printf, 1, 2)));
#endif

#define NPROC 3                 /* nombre de processus à utiliser */
#define NMSG 10                 /* nombre de messages à afficher */

/* Les différentes parties du message.
 */
const char * const words[NPROC] = { "IUT ", "Belfort ", "Montbéliard\n" };

const char *message;            /* partie du message à afficher */
pid_t next;                     /* PID du processus suivant */
int debug;                      /* active les messages de debug si != 0 */

/* Comme fprintf(stderr, ....), mais le message n'est écrit que si debug != 0.
 */
void pr_debug(const char *format, ...)
{
    if (debug) {
        va_list ap;
        va_start(ap, format);
        vfprintf(stderr, format, ap);
        va_end(ap);
    }
}

/* Gestionnaire de signal pour les processus fils : affiche le message puis
 * envoie le signal SIGUSR1 au processus suivant.
 */
void afficheur(unused int sig)
{
    write(STDOUT_FILENO, message, strlen(message));
    kill(next, SIGUSR1);
}

/* Gestionnaie de signal pour le processus père : envoie le signal SIGUSR1 au
 * premier processus fils tant que msgcount < NMSG.  Tue tous les processus
 * sinon.
 */
void controlleur(unused int sig)
{
    static int msgcount = 0;
    if (msgcount++ < NMSG) {
        if (debug) {
            char msg[] = "# 00 # ";
            msg[2] += ((msgcount - 1) / 10) % 10;
            msg[3] += (msgcount - 1) % 10;
            write(STDERR_FILENO, msg, sizeof msg);
        }
        /* envoie SIGUSR1 au premier processus */
        kill(next, SIGUSR1);
    } else {
        if (debug) {
            char msg[] = "# limite atteinte, on tue les processus fils\n";
            write(STDERR_FILENO, msg, sizeof msg);
        }
        /* limite atteinte, on tue les processus fils */
        kill(0, SIGUSR2);
    }
}

int main(int argc, char *argv[])
{
    debug = argc > 1 && strcmp(argv[1], "-debug") == 0;
    pr_debug("# mode debug activé (NPROC = %d ; NMSG = %d)\n", NPROC, NMSG);

    /* Mise en place du gestionnaire pour SIGUSR1, qui sera hérité par les
     * processus fils. */
    struct sigaction action = { .sa_handler = afficheur, .sa_flags = 0 };
    sigemptyset(&action.sa_mask);
    sigaction(SIGUSR1, &action, NULL);

    /* Lancement des processus fils, en initialisant les variables `next' et
     * `message'. */
    next = getpid();
    for (int i = NPROC - 1; i >= 0; i--) {
        message = words[i];
        pid_t fils = fork();
        switch (fils) {
        case -1:
            perror("fork");
            kill(0, SIGABRT);
            break;
        case 0:
            while (1)
                pause();
            exit(0);
        }
        if (debug) {
            char *nl = strchr(message, '\n');
            pr_debug("# processus %d (message = \"%.*s%s\", next = %d)\n",
                     (int)fils, (int)strlen(message) - !!nl, message,
                     "\\n" + 2 * !nl, (int)next);
        }
        next = fils;
    }
    pr_debug("# proc père %d (next = %d)\n", (int)getpid(), (int)next);

    /* Mise en place du gestionnaire pour SIGUSR1 pour le processus maître.  Le
     * signal SIGUSR2 est ignoré (utilisé pour tuer les processus fils). */
    action.sa_handler = controlleur;
    sigaction(SIGUSR1, &action, NULL);
    action.sa_handler = SIG_IGN;
    sigaction(SIGUSR2, &action, NULL);

    /* L'affichage des message est démarré en envoyant un premier SIGUSR1 au
     * processus maître. */
    raise(SIGUSR1);

    /* Attente de la fin des processus fils. */
    pid_t fils;
    int status;
    while ((fils = wait(&status)) > 0 || errno == EINTR) {
        if (fils > 0) {
            if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                pr_debug("# processus %d tué par le signal %d (%s)\n",
                         (int)fils, sig, strsignal(sig));
            } else if (WIFEXITED(status)){
                pr_debug("# processus %d terminé avec le code %d\n",
                         (int)fils, WEXITSTATUS(status));
            } else {
                pr_debug("# processus %d, état inconnu (0x%04x)\n",
                         (int)fils, status);
            }
        }
    }

    pr_debug("# terminé.\n");
    return EXIT_SUCCESS;
}
