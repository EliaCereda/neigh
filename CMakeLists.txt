cmake_minimum_required(VERSION 3.9)
project(neigh)

set(CMAKE_C_STANDARD 11)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

add_executable(neigh src/main.c src/dist_matrix.c src/io.c src/io.h)