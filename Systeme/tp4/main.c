#include <stdio.h>
#include <unistd.h>

void affiche_car(const char *prefixe){
    __pid_t pid = getpid();
    __pid_t ppid = getppid();
    printf("\n%s", prefixe);
    printf(", Processus numéro : %d. Numéro du processus père : %d.\n", pid, ppid);
}

void create_fils() {
    affiche_car(fork());
}

void create_petitfils() {
    affiche_car(fork());
}

int main(){
    affiche_car("Linux");
    create_fils();
    create_petitfils();
    return 0;
}



