#include <stdio.h>
#include <unistd.h>

void creerterm(char *string) {
    char * args[3];
    args[0] = "xterm ";
    args[1] = string;
    args[2] = NULL;
    execv("/usr/bin/xterm", args);
    perror("execev");
}

int main (int argc, char* argv[]) {
    creerterm(argv[1]);
    return -1;
}