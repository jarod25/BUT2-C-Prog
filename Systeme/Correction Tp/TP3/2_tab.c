/*
 * QUESTIONS
 *
 * On peut afficher le contenu du fichier avec la commande od(1).
 * Pour un affichage en décimal:        od -i /tmp/tab.bin
 *                      ou bien:        od -t dI /tmp/tab.bin
 * Pour un affichage en hexadécimal:    od -t xI /tmp/tab.bin
 */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define N 100                   /* taille du tableau */
#define OUT "/tmp/tab.bin"      /* nom du ficher de sortie */

int main(void)
{
    /* Création du tableau
     */
    int a[N];
    for (int i = 0; i < N; i++)
        a[i] = i;

    /* Ouverture du fichier en écriture (O_WRONLY).  Si le fichier existe, il
     * est tronqué (O_TRUNC), sinon il est créé (O_CREAT).
     * Comme le fichier est potentiellement créé, il faut spécifier les droits
     * d'accès en 3ème paramètre.
     *
     * On aurait pu, de manière équivalente, écrire
     * int fd = creat(OUT, S_IRUSR | S_IWUSR);
     */
    int fd = open(OUT, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open()");
        return 1;
    }

    /* Écriture du tableau
     */
    ssize_t count = sizeof a;   /* taille du tableau, en octets */
    if (write(fd, a, count) != count) {
        perror("write()");
        return 1;
    }

    /* On vérifie également la valeur de retour de close().  Une erreur
     * d'écriture pourrait n'être remontée qu'à ce moment-là.
     */
    if (close(fd) == -1) {
        perror("close()");
        return 1;
    }
    return 0;
}
