cmake_minimum_required(VERSION 3.23)
project(Systeme C)

set(CMAKE_C_STANDARD 11)

add_executable(Systeme
        "Correction Tp/TP2/1_copie_block.c"
        "Correction Tp/TP2/1_copie_char.c"
        "Correction Tp/TP2/2_binhexasc.c"
        "Correction Tp/TP2/3_copie.c"
        "Correction Tp/TP3/1_cat.c"
        "Correction Tp/TP3/1_cat_multiple.c"
        "Correction Tp/TP3/2_tab.c"
        "Correction Tp/TP3/3_copie.c"
        "Correction Tp/TP4-5/1_fork_simple.c"
        "Correction Tp/TP4-5/2_petits-fils.c"
        "Correction Tp/TP4-5/3_creerterm.c"
        "Correction Tp/TP4-5/4_creerterm_root.c"
        "Correction Tp/TP4-5/5_system.c"
        "Correction Tp/TP4-5/verif_system.c"
        "Correction Tp/TP6/1_execute_bas.c"
        "Correction Tp/TP6/1_execute_haut.c"
        "Correction Tp/TP6/2_sigaction_bsd.c"
        "Correction Tp/TP6/2_sigaction_sysv.c"
        "Correction Tp/TP6/2_signal_bsd.c"
        "Correction Tp/TP6/2_signal_sysv.c"
        "Correction Tp/TP6/3_synchro.c"
        "Correction Tp/TP7/1_threads.c"
        "Correction Tp/TP7/2_calcul.c"
        "Correction Tp/TP7/3_concur.c"
        mini-projet/compare.c
        mini-projet/xxd.c
        "Prog_system/Prog system/revisions/abs.c"
        "Prog_system/Prog system/revisions/divise.c"
        "Prog_system/Prog system/revisions/exo1.c"
        "Prog_system/Prog system/revisions/exo2.c"
        "Prog_system/Prog system/revisions/exo3.c"
        "Prog_system/Prog system/revisions/exo4.c"
        "Prog_system/Prog system/revisions/exo5.c"
        "Prog_system/Prog system/revisions/facto.c"
        "Prog_system/Prog system/revisions/somme.c"
        "Prog_system/Prog system/revisions/suiteV.c"
        "Prog_system/Prog system/revisions/triCroissant.c"
        "Prog_system/Prog system/TP_n°1/binhexasc.c"
        "Prog_system/Prog system/TP_n°1/contatenation.c"
        "Prog_system/Prog system/TP_n°1/impression.c"
        "Prog_system/Prog system/TP_n°2/copieFile.c"
        "Prog_system/Prog system/TP_n°2/formatBin.c"
        "Prog_system/Prog system/TP_n°2/impression.c"
        "Prog_system/Prog system/TP_n°3/affiche.c"
        "Prog_system/Prog system/TP_n°4/createTerm.c"
        "Prog_system/Prog system/TP_n°5/redirect.c"
        td1/exo1.c
        td1/exo2.c
        td1/exo3.c
        td1/exo4.c
        td1/exo5.c
        tp2/binhexasc.c
        tp2/main.c
        tp3/copie.c
        tp3/main.c
        tp4/main.c
        tp5/main.c)
