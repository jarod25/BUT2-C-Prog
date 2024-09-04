#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>

int mon_system(const char *commande)
{
    fprintf(stderr, "FIXME: not implemented: %s(\"%s\")\n", __func__, commande);
    return 0;
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
