#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#define N 60

int write_bin(const char *file, int a[], int n)
{
    int res = 0;
    FILE *fp = fopen(file, "w");
    if (fp == NULL) {
        perror(__func__);
        return 1;
    }
    if (fwrite(a, sizeof a[0], n, fp) != (size_t)n) {
        perror(__func__);
        res = 1;
    }
    if (fclose(fp) != 0) {
        perror(__func__);
        res = 1;
    }
    return res;
}

int write_dec(const char *file, int a[], int n)
{
    int res = 0;
    FILE *fp = fopen(file, "w");
    if (fp == NULL) {
        perror(__func__);
        return 1;
    }
    for (int i = 0; i < n; i++)
        if (fprintf(fp, "%3d\n", a[i]) < 0) {
            perror(__func__);
            res = 1;
            break;
        }
    if (fclose(fp) != 0) {
        perror(__func__);
        res = 1;
    }
    return res;
}

int write_hex(const char *file, int a[], int n)
{
    int res = 0;
    FILE *fp = fopen(file, "w");
    if (fp == NULL) {
        perror(__func__);
        return 1;
    }
    for (int i = 0; i < n; i++)
        if (fprintf(fp, "%08x\n", a[i]) < 0) {
            perror(__func__);
            res = 1;
            break;
        }
    if (fclose(fp) != 0) {
        perror(__func__);
        res = 1;
    }
    return res;
}

int main(void)
{
    int a[N];
    for (int i = 0; i < N; i++)
        a[i] = i + 1;

    printf("Valeurs:");
    for (int i = 0; i < N; i++)
        printf(" %02d", a[i]);
    putchar('\n');

    // utilisation de umask(2) pour fixer les droits d'accès des fichiers créés
    umask(S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
    
    int res = 0;
    if (write_bin("res.bin", a, N))
        res += 1;
    if (write_dec("res.ascii", a, N))
        res += 2;
    if (write_hex("res.hex", a, N))
        res += 4;
    
    return res;
}
