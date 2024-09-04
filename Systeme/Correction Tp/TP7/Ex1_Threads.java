class Ex1_Threads extends Thread {

    private String nom_;

    Ex1_Threads(String nom) {
        nom_ = nom;
    }

    /* La méthode exécutée par les différents threads.
     */
    public void run() {
        System.out.println("Bonjour " + nom_ + " !");
    }

    static final int NTHREADS = 3;
    static final String[] NOMS = { "toto", "titi", "tata" };

    public static void main(String[] args) {
        System.out.println("Go go go...");

        // Lance les NTHREADS threads.  Le i-ème thread reçoit la chaîne NOMS[i]
        // en paramètre.
        Ex1_Threads[] threads = new Ex1_Threads[NTHREADS];
        for (int i = 0; i < NTHREADS; i++) {
            threads[i] = new Ex1_Threads(NOMS[i]);
            threads[i].start();
        }

        // On attend que les threads lancés ci-dessus se terminent.
        for (Ex1_Threads th: threads) {
            try {
                th.join();
            } catch (InterruptedException e) {
            }
        }

        System.out.println("Done.");
    }
}
