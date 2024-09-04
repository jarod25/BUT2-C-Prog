class Ex2_Calcul extends Thread {

    /* Tableau de réels, mais avec une limite de taille plus grande que les
     * tableau standards de Java : près de 4Eo (4.6e18) au lieu de 2Go (2.1e9).
     */
    static class Tableau {
        private static final int BLOCK_SIZE = 0x7FFF0000;
        private float[][] a;

        Tableau(long taille) {
            if (taille > (long)BLOCK_SIZE * BLOCK_SIZE)
                throw new IndexOutOfBoundsException("taille trop grande");
            int size0 = (int)(taille / BLOCK_SIZE);
            int size1 = (int)(taille % BLOCK_SIZE);
            if (size1 == 0)
                size1 = BLOCK_SIZE;
            else
                size0 += 1;
            a = new float[size0][];
            for (int i = 0; i < size0 - 1; i++)
                a[i] = new float[BLOCK_SIZE];
            a[size0 - 1] = new float[size1];
        }

        float get(long i) {
            return a[(int)(i / BLOCK_SIZE)][(int)(i % BLOCK_SIZE)];
        }
        void set(long i, float val) {
            a[(int)(i / BLOCK_SIZE)][(int)(i % BLOCK_SIZE)] = val;
        }
    }

    private Tableau a_;
    private long start_;
    private long end_;
    private float sum_;

    Ex2_Calcul(Tableau a, long start, long end) {
        a_ = a;
        start_ = start;
        end_ = end;
    }

    /* Méthode de calcul des threads: calcule la somme des éléments
     * a_[start], a_[start_ + 1], ..., a_[end_]
     */
    public void run() {
        sum_ = 0.0f;
        for (long i = start_; i < end_; i++)
            sum_ += a_.get(i);
    }

    float getSum() {
        return sum_;
    }

    static Tableau initTab(long taille) {
        Tableau res = new Tableau(taille);
        for (long i = 0; i < taille; i++)
            res.set(i, i);
        return res;
    }

    /* Retourne une chaîne de caractères représentant la valeur du paramètre
     * size, avec le préfixe IEC (K, M, G, etc.) le plus adapté.
     */
    static String humanReadable(long size) {
        final String[] units = { "", "K", "M", "G", "T", "P", "E" };
        double val = size;
        int suffix;
        for (suffix = 0; suffix < 7 && val >= 1024; suffix++)
            val /= 1024;
        val = Math.ceil(10.0 * val) / 10.0;
        int prec =
            suffix > 0 && String.format("%.1f", val).length() <= 3 ? 1 : 0;
        String format = "%." + prec + "f%s";
        return String.format(format, val, units[suffix]);
    }

    /* Stocke l'heure courante dans clock1 ([0]:temps CPU, [1]: temps écoulé).
     * Si clock0 est différent de null, retourne une chaîne de caractères
     * représentant les différences (clock1[0] - clock0[0]) et
     * (clock1[1] - clock0[0]).
 */
    static String timestamp(long[] clock0, long[] clock1) {
        String result = null;
        com.sun.management.OperatingSystemMXBean osMXBean =
            (com.sun.management.OperatingSystemMXBean)
            java.lang.management.ManagementFactory.getOperatingSystemMXBean();
        clock1[0] = osMXBean.getProcessCpuTime();
        clock1[1] = System.nanoTime();
        if (clock0 != null) {
            double cpu = (clock1[0] - clock0[0]) / 1e9;
            double elapsed = (clock1[1] - clock0[1]) / 1e9;
            String unit = "s";
            double ratio = 100.0 * cpu / elapsed;
            if (cpu < 1.0 && elapsed < 1.0) {
                cpu *= 1000.0;
                elapsed *= 1000.0;
                unit = "ms";
            }
            result = String.format("%.4g%s cpu, %.4g%s elapsed, %.1f%%",
                                   cpu, unit, elapsed, unit, ratio);
        }
        return result;
    }

    public static void main(String[] args) {
        long[] clock0 = new long[2];
        timestamp(null, clock0);

        // Récupération des paramètres.
        if (args.length != 2) {
            System.err.println("Usage: Ex2_Calcul taille_tableau nb_threads");
            System.exit(1);
        }
        long taille_tab = Long.parseLong(args[0]);
        int nthreads = Integer.parseInt(args[1]);

        System.err.printf("# taille_tab = %d; nthreads = %d\n",
                          taille_tab, nthreads);
        if (taille_tab < 1 || nthreads < 1) {
            System.err.println("Paramètres invalides.");
            System.exit(1);
        }

        // Initialisation du tableau et création des threads.
        System.err.printf("# initialisation (%s)...\n",
                          humanReadable(taille_tab * (Float.SIZE / 8)));
        Tableau tab = null;
        try {
            tab = initTab(taille_tab);
        } catch (OutOfMemoryError e) {
            System.err.println(
"ERREUR: " + e.getMessage() + "\n" +
"\n" +
"  Réduisez la taille du tableau, ou bien essayez d'augmenter la taille\n" +
"  maximum de la Java VM avec le paramètre -Xmxn.\n" +
"  Par exemple:\n" +
"                  java -Xmx32G Ex2_Calcul ...");
            System.exit(1);
        }
        Ex2_Calcul[] threads = new Ex2_Calcul[nthreads];
        for (int i = 0; i < nthreads; i++) {
            long start = i * taille_tab / nthreads;
            long end = (i + 1) * taille_tab / nthreads;
            threads[i] = new Ex2_Calcul(tab, start, end);
            if (false) {
                System.err.printf("# * thread %d: %d [%d..%d)\n",
                                  i, end - start, start, end);
            }
        }

        long[] clock1 = new long[2];
        System.err.println("# terminé (" + timestamp(clock0, clock1) + ")");

        // Lancement des threads de calcul, puis accumulation des résultats.
        System.err.println("# calcul...");
        for (Ex2_Calcul th: threads)
            th.start();
        float somme = 0.0f;
        for (Ex2_Calcul th: threads) {
            try {
                th.join();
            } catch (InterruptedException e) {
            }
            somme += th.getSum();
        }

        long[] clock2 = new long[2];
        System.err.println("# terminé (" + timestamp(clock1, clock2) + ")");

        // Affichage du résultat et finalisation.
        System.out.println("SOMME = " + somme);

        long[] clock3 = new long[2];
        System.err.println("# temps total: " + timestamp(clock0, clock3));
    }
}
