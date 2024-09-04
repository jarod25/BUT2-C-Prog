cmake_minimum_required(VERSION 3.23)
project(tp2 C)

set(CMAKE_C_STANDARD 99)

add_executable(tp2 actif_client.c actif_serveur.c passif_client.c passif_serveur.c)
