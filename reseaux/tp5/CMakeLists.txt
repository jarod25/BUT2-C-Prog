cmake_minimum_required(VERSION 3.23)
project(tp5 C)

set(CMAKE_C_STANDARD 99)

add_executable(tp5 client.c, serveur.c)
