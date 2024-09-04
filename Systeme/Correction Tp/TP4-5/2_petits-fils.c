#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void affiche_car(const char *prefixe)
{
    printf("[%s] Processus numéro : %-5d  Numéro du processus père : %-5d\n",
           prefixe, (int)getpid(), (int)getppid());
}

/* Attend la fin de tous les processus fils */
void attente()
{
    int status;
    pid_t fils;
    do {
        fils = wait(&status);
        if (fils != -1) {
            printf("Processus %d: mon fils %d s'est terminé ",
                   (int)getpid(), (int)fils);
            if (WIFEXITED(status))
                printf("normalement avec le code %d\n", WEXITSTATUS(status));
            else
                printf("anormalement\n");
        }
    } while (fils != -1 || errno == EINTR);
}

int main(void)
{
    int i;
    affiche_car("PERE");
    for (i = 0; i < 2; i++) {
        if (fork() == 0) {
            /* Dans le fils */
            affiche_car("FILS");
            if (fork() == 0) {
                /* Dans le petit-fils (le fils du fils) */
                affiche_car("PETIT_FILS");
                exit(1);
            }
            attente();          /* le fils attend son fils (le petit fils) */
            exit(0);            /* puis se termine */
        }
    }
    /* Dans le père uniquement */
    attente();                  /* le père attend le fils */
    return 0;
}
