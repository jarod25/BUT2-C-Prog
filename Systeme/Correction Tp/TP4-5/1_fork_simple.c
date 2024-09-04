#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void affiche_car(const char *prefixe)
{
    printf("[%s] Processus numéro : %-5d  Numéro du processus père : %-5d\n",
           prefixe, (int)getpid(), (int)getppid());
}

int main(void)
{
    affiche_car("PERE");
    if (fork() == 0) {
        /* Processus fils */
        // sleep(5);
        affiche_car("FILS");
    }
    return 0;
}
