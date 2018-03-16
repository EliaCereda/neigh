cmake_minimum_required(VERSION 3.9)
project(neigh)

set(CMAKE_C_STANDARD 11)

add_executable(neigh src/main.c src/dist_matrix.c)