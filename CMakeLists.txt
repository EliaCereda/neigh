cmake_minimum_required(VERSION 3.9)
project(neigh)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

add_executable(neigh src/main.c src/dist_matrix.c src/dist_matrix.h src/io.c src/io.h src/neighbour_joining.c src/neighbour_joining.h src/phylogenetic_tree.c src/phylogenetic_tree.h src/utilities.h)
