cmake_minimum_required(VERSION 3.9)
include(CMakeDependentOption)

project(neigh)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

# Source files
set(neigh_SOURCES
        src/main.c
        src/dist_matrix.c src/dist_matrix.h
        src/io.c src/io.h
        src/neighbour_joining.c src/neighbour_joining.h
        src/phylogenetic_tree.c src/phylogenetic_tree.h
        src/utilities.c src/utilities.h
    )

set(neigh_LIBS )

# Gengetopt
find_program(GENGETOPT_EXECUTABLE gengetopt)

if (NOT GENGETOPT_EXECUTABLE)
    message(FATAL_ERROR "gengetopt not found - aborting")
endif()

set(neigh_GGO src/options.ggo)
set(neigh_CMDLINE ${CMAKE_CURRENT_BINARY_DIR}/cmdline.c ${CMAKE_CURRENT_BINARY_DIR}/cmdline.h)

add_custom_command(OUTPUT ${neigh_CMDLINE}
                   COMMAND ${GENGETOPT_EXECUTABLE}
                   ARGS
                        --input=${CMAKE_CURRENT_SOURCE_DIR}/${neigh_GGO}
                   DEPENDS ${neigh_GGO}
                  )

set(neigh_SOURCES ${neigh_SOURCES} ${neigh_GGO} ${neigh_CMDLINE})

# Graphviz visualization
find_library(CGRAPH_LIB cgraph)
find_library(GVC_LIB gvc)
find_path(GVC_INCLUDE_DIR graphviz/gvc.h)

cmake_dependent_option(HAS_GRAPHVIZ "Enable tree visualization with Graphviz" ON "CGRAPH_LIB;GVC_LIB" OFF)

if (HAS_GRAPHVIZ)
    message(STATUS "Graphviz support enabled.")

    set(neigh_SOURCES ${neigh_SOURCES} src/viz.c src/viz.h)
    set(neigh_LIBS    ${neigh_LIBS}    ${CGRAPH_LIB} ${GVC_LIB})

    include_directories(SYSTEM ${GVC_INCLUDE_DIR})
else()
    message(WARNING "Compiling without Graphviz support.")
endif()

# config.h header
configure_file(src/config.h.in config.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(neigh ${neigh_SOURCES})
target_link_libraries(neigh ${neigh_LIBS})
