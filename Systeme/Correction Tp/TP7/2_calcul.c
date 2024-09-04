#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct timespec time_type;

/* Les paramètres de calcul : on veut la somme des éléments
 * a[start], a[start + 1], ..., a[end - 1].
 */
struct param {
    float *a;                   /* le tableau */
    size_t start;               /* premier élément */
    size_t end;                 /* dernier élément (plus 1) */
};

/* Comme malloc(), mais quitte le programme avec un message d'erreur en cas
 * d'échec.
 */
void *xmalloc(size_t size)
{
    void *res = malloc(size);
    if (res == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return res;
}

/* Retourne une chaîne de caractères représentant la valeur du paramètre size,
 * avec le préfixe IEC (K, M, G, etc.) le plus adapté.
 */
const char *human_readable(size_t size)
{
    static const char * const units[7] = { "", "K", "M", "G", "T", "P", "E" };
    static char result[100];
    double val = size;
    int suffix;
    for (suffix = 0; suffix < 7 && val >= 1024; suffix++)
        val /= 1024;
    val = ceil(10.0 * val) / 10.0;
    int prec = suffix > 0 && snprintf(NULL, 0, "%.1f", val) <= 3 ? 1 : 0;
    snprintf(result, sizeof result, "%.*f%s", prec, val, units[suffix]);
    return result;
}

/* Stocke l'heure courante dans *cpu1 (temps CPU) et *clock1 (temps écoulé).
 * Si cpu0 et clock0 sont différents de NULL, retourne une chaîne de caractères
 * représentant les différences (*cpu1 - *cpu0) et (*clock1 - *clock0).
 */
const char *timestamp(const time_type *cpu0, const time_type *clock0,
                      time_type *cpu1, time_type *clock1)
{
    static char result[100];
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, cpu1);
    clock_gettime(CLOCK_MONOTONIC, clock1);
    if (cpu0 && clock0) {
        double cpu = cpu1->tv_sec - cpu0->tv_sec +
            (cpu1->tv_nsec - cpu0->tv_nsec) / 1e9;
        double elapsed = clock1->tv_sec - clock0->tv_sec +
            (clock1->tv_nsec - clock0->tv_nsec) / 1e9;
        const char *unit = "s";
        double ratio = 100.0 * cpu / elapsed;
        if (cpu < 1.0 && elapsed < 1.0) {
            cpu *= 1000.0;
            elapsed *= 1000.0;
            unit = "ms";
        }
        snprintf(result, sizeof result, "%.4g%s cpu, %.4g%s elapsed, %.1f%%",
                 cpu, unit, elapsed, unit, ratio);
    }
    return result;
}

/* Construit et initialise un tableau de la taille donnée en paramètre.
 */
float *init_tab(size_t taille)
{
    float *res = xmalloc(taille * sizeof *res);
    for (size_t i = 0; i < taille; i++)
        res[i] = i;
    return res;
}

/* Fonction de calcul.  Le paramètre arg doit pointer vers une donnée de type
 * struct param.  La somme des éléments du tableau est calculée suivant le
 * paramètres donnés.
 * Retourne un pointeur vers le résultat (qui aura été alloué avec malloc()).
 */
void *calcul(void *arg)
{
    struct param *param = arg;
    float sum = 0.0;
    for (size_t i = param->start; i < param->end; i++)
        sum += param->a[i];
    float *res = xmalloc(sizeof *res);
    *res = sum;
    return res;
}

int main(int argc, char *argv[])
{
    time_type cpu0, clock0;
    timestamp(NULL, NULL, &cpu0, &clock0);

    /* Récupération des paramètres. */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s taille_tableau nb_threads\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    size_t taille_tab = atol(argv[1]);
    int nthreads = atoi(argv[2]);

    fprintf(stderr, "# taille_tab = %zu; nthreads = %d\n", taille_tab, nthreads);
    if (taille_tab < 1 || nthreads < 1) {
        fprintf(stderr, "Paramètres invalides.\n");
        exit(EXIT_FAILURE);
    }

    /* Initialisation du tableau et génération des paramètres des threads de
     * calcul. */
    fprintf(stderr, "# initialisation (%s)...\n",
            human_readable(taille_tab * sizeof(float)));
    float *tab = init_tab(taille_tab);
    struct param *params = xmalloc(nthreads * sizeof *params);
    for (int i = 0; i < nthreads; i++) {
        params[i].a = tab;
        params[i].start = i * taille_tab / nthreads;
        params[i].end = (i + 1) * taille_tab / nthreads;
        if (0) {
            fprintf(stderr, "# * thread %d: %zu [%zu..%zu)\n",
                    i, params[i].end - params[i].start,
                    params[i].start, params[i].end);
        }
    }

    time_type cpu1, clock1;
    fprintf(stderr, "# terminé (%s)\n",
            timestamp(&cpu0, &clock0, &cpu1, &clock1));

    /* Lancement des threads de calcul, puis accumulation des résultats. */
    fprintf(stderr, "# calcul...\n");
    pthread_t *threads = xmalloc(nthreads * sizeof *threads);
    for (int i = 0; i < nthreads; i++) {
        if (pthread_create(&threads[i], NULL, calcul, &params[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    float somme = 0.0;
    for (int i = 0; i < nthreads; i++) {
        void *res;
        if (pthread_join(threads[i], &res) != 0) {
            perror("pthread_join");
        } else {
            somme += *(float *)res;
            free(res);
        }
    }

    time_type cpu2, clock2;
    fprintf(stderr, "# terminé (%s)\n",
            timestamp(&cpu1, &clock1, &cpu2, &clock2));

    /* Affichage du résultat et finalisation. */
    printf("SOMME   = %g\n", somme);

    time_type cpu3, clock3;
    fprintf(stderr, "# temps total: %s\n",
            timestamp(&cpu0, &clock0, &cpu3, &clock3));

    free(threads);
    free(params);
    free(tab);
}
