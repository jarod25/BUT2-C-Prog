#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>

int mon_system(const char *commande)
{
    /* commande NULL : rien à faire */
    if (commande == NULL)
        return 1;

    /* ignore les signaux SIGINT et SIGQUIT ;
     * sauvegarde des anciens traitements pour une restauration ultérieure */
    struct sigaction sa_ign, sauv_int, sauv_quit;
    sa_ign.sa_handler = SIG_IGN;
    sa_ign.sa_flags = 0;
    sigemptyset(&sa_ign.sa_mask);
    sigaction(SIGINT, &sa_ign, &sauv_int);
    sigaction(SIGQUIT, &sa_ign, &sauv_quit);

    /* création du processus fils */
    pid_t fils = fork();
    if (fils == 0) {            /* processus fils */
        /* restauration de la gestion des signaux SIGINT et SIGQUIT pour le
         * processus fils */
        sigaction(SIGINT, &sauv_int, NULL);
        sigaction(SIGQUIT, &sauv_quit, NULL);
        /* exécution de la commande */
        execl("/bin/sh", "sh", "-c", commande, (char *)NULL);
        _exit(127);             /* execl() a échoué */
    }

    int statut;
    if (fils == -1) {
        statut = -1;            /* fork() a échoué */
    } else {
        /* attente du processus fils et récupération du statut */
        while (waitpid(fils, &statut, 0) == -1) {
            if (errno != EINTR) { /* waitpid() a échoué */
                statut = -1;
                break;
            }
        }
    }

    /* restauration de la gestion des signaux SIGINT et SIGQUIT */
    sigaction(SIGINT, &sauv_int, NULL);
    sigaction(SIGQUIT, &sauv_quit, NULL);

    /* le statut du processus fils est retourné */
    return statut;
}

void verification_system(const char *commande)
{
    int a = system(commande);
    int b = mon_system(commande);
    fprintf(stderr, "%s: \"%s\" (%d, %d)\n",
            a == b ? "OK  " : "FAIL", commande ? commande : "(null)", a, b);
}

void verification_system_limit(int resource, int valeur, const char *commande)
{
    struct rlimit oldrl, newrl;
    if (getrlimit(resource, &oldrl) == -1) {
        perror("getrlimit");
        return;
    }
    newrl = oldrl;
    newrl.rlim_cur = valeur;
    if (setrlimit(resource, &newrl) == -1) {
        perror("setrlimit");
        return;
    }
    verification_system(commande);
    setrlimit(resource, &oldrl);
}

int main(int argc, char *argv[])
{
    if (argc >= 2) {
        for (int i = 1; i < argc; i++)
            verification_system(argv[i]);
    } else {
        const char *cmds[] = {
            "",                       /* empty command */
            "true",                   /* successful command */
            "false",                  /* failing command */
            "ls / > /dev/null",       /* another command */
            "exec 2>/dev/null; plop", /* non-existent command */
            "kill -HUP $$",           /* killed by SIGHUP */
            "kill -INT $$",           /* killed by SIGINT */
            "kill -QUIT $$",          /* killed by SIGQUIT */
            "kill -INT $PPID",        /* send SIGINT to main process */
            "kill -QUIT $PPID",       /* send SIGQUIT to main process */
            NULL                      /* NULL command */
        };

        for (int i = 0; i == 0 || cmds[i - 1] != NULL; i++) {
            verification_system(cmds[i]);
        }
        verification_system_limit(RLIMIT_NPROC, 0, ": failed fork");
        verification_system_limit(RLIMIT_AS, 0, ": failed exec");
    }
}
