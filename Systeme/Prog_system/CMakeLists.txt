cmake_minimum_required(VERSION 3.23)
project(Prog_system C)

set(CMAKE_C_STANDARD 11)

add_executable(Prog_system
        "Prog system/revisions/abs.c"
        "Prog system/revisions/divise.c"
        "Prog system/revisions/exo1.c"
        "Prog system/revisions/exo2.c"
        "Prog system/revisions/exo3.c"
        "Prog system/revisions/exo4.c"
        "Prog system/revisions/exo5.c"
        "Prog system/revisions/facto.c"
        "Prog system/revisions/somme.c"
        "Prog system/revisions/suiteV.c"
        "Prog system/revisions/triCroissant.c"
        "Prog system/TP_n°1/binhexasc.c"
        "Prog system/TP_n°1/contatenation.c"
        "Prog system/TP_n°1/impression.c"
        "Prog system/TP_n°2/copieFile.c"
        "Prog system/TP_n°2/formatBin.c"
        "Prog system/TP_n°2/impression.c"
        "Prog system/TP_n°3/affiche.c"
        "Prog system/TP_n°4/createTerm.c"
        "Prog system/TP_n°5/redirect.c")
