cmake_minimum_required(VERSION 3.24)
project(tp4 C)

set(CMAKE_C_STANDARD 11)

add_executable(tp4 client.c serveur1.c serveur2.c serveurbase.c serveur3.c serveurthreads.c)
