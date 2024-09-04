#include <pthread.h>
#include <stdio.h>

#define NTHREADS 3

/* Quelques noms.. */
const char * const NOMS[NTHREADS] = {
    "toto", "titi", "tata"
};

/* La fonction à exécuter par les différents threads.
 */
void *ma_jolie_fonction(void *arg)
{
    const char *param = arg;
    printf("Bonjour %s !\n", param);
    return NULL;
}

int main(void)
{
    printf("Go go go...\n");

    /* Lance NTHREADS threads.  Le i-ème thread reçoit la chaîne NOMS[i] en
     * paramètre. */
    pthread_t threads[NTHREADS];
    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_create(&threads[i], NULL, ma_jolie_fonction,
                           (void *)NOMS[i]) != 0)
            perror("pthread_create");
    }

    /* On attend que les threads lancés ci-dessus se terminent. */
    for (int i = 0; i < NTHREADS; i++)
        pthread_join(threads[i], NULL);

    printf("Done.\n");

    return 0;
}
