/*
 * Cinq threads font en même temps des mises à jour sur une donnée partagée.
 * Sans précaution, on voit que le résultat ne correspond pas au résultat
 * attendu.
 *
 * Une solution pour résoudre le problème est d'utiliser un verrou (mutex) pour
 * protéger la section critique (la partie du code qui accède à la donnée
 * partagée).
 *
 * En Java, cela peut se faire avec un bloc "synchronized".  Cf. les lignes 37
 * et 39 ci dessous.
 */

class Ex3_Concur extends Thread {

    static class Value {
        private long v_;
        Value() {
            v_ = 0;
        }
        void inc(long x) {
            v_ += x;
        }
        long get() {
            return v_;
        }
    };

    private Value v_;
    private long n_;
    private long i_;

    Ex3_Concur(Value v, long n, long i) {
        v_ = v;
        n_ = n;
        i_ = i;
    }

    /* La méthode de calcul des différents threads.
     */
    public void run() {
        for (long i = 0; i < n_; i++) {
            // synchronized (v_) {
                v_.inc(i_);
            // }
        }
    }

    static final int NTHREADS = 5;

    public static void main(String[] args) {
        Value v = new Value();
        long param_n = 100000;
        long param_i = 3;

        System.out.printf("# n = %d, i = %d, nthreads = %d\n",
                          param_n, param_i, NTHREADS);

        // Lance les NTHREADS threads.
        Ex3_Concur[] threads = new Ex3_Concur[NTHREADS];
        for (int i = 0; i < NTHREADS; i++) {
            threads[i] = new Ex3_Concur(v, param_n, param_i);
            threads[i].start();
        }

        // On attend que les threads lancés ci-dessus se terminent.
        for (Ex3_Concur th: threads) {
            try {
                th.join();
            } catch (InterruptedException e) {
            }
        }

        System.out.println("Résultat ..........: " + v.get());
        System.out.println("Résultat théorique : " +
                           (NTHREADS * param_n * param_i));
    }
}
