/*
 * Cinq threads font en même temps des mises à jour sur une donnée partagée.
 * Sans précaution, on voit que le résultat ne correspond pas au résultat
 * attendu.
 *
 * Une solution pour résoudre le problème est d'utiliser un verrou (mutex) pour
 * protéger la section critique (la partie du code qui accède à la donnée
 * partagée).
 *
 * Un mutex est de type pthread_mutex_t, s'acquiert par pthread_mutex_lock() et
 * se relâche par pthread_mutex_unlock().  Cf. les lignes 27, 35 et 37
 * ci-dessous.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 5

struct param {
    long *p;
    long n;
    long i;
};

/* pthread_mutex_t verrou = PTHREAD_MUTEX_INITIALIZER; */

/* La fonction de calcul des threads.
 */
void *calcul(void *arg)
{
    struct param *param = arg;
    for (long i = 0; i < param->n; i++) {
        /* pthread_mutex_lock(&verrou); */
        *param->p += param->i;
        /* pthread_mutex_unlock(&verrou); */
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    long v = 0;                 /* la variable partagée */
    struct param param = {      /* les paramètres des threads */
        .p = &v,
        .n = 100000,
        .i = 3,
    };

    if (argc > 1)
        param.n = atol(argv[1]);
    if (argc > 2)
        param.i = atol(argv[2]);
    if (argc > 3 || param.n == 0 || param.i ==0) {
        fprintf(stderr, "Usage: %s [n [i]]\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("# n = %ld, i = %ld, nthreads = %d\n", param.n, param.i, NTHREADS);

    /* Lance les NTHREADS threads. */
    pthread_t threads[NTHREADS];
    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_create(&threads[i], NULL, calcul, &param) != 0)
            perror("pthread_create");
    }

    /* On attend que les threads lancés ci-dessus se terminent. */
    for (int i = 0; i < NTHREADS; i++)
        pthread_join(threads[i], NULL);

    printf("Résultat ..........: %ld\n", v);
    printf("Résultat théorique : %ld\n", NTHREADS * param.n * param.i);

    return EXIT_SUCCESS;
}
