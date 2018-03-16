#include "io.h"
#include <stdio.h>

dist_matrix *load_file(const char *file_name) {
    FILE *f = fopen(file_name, "r");

    if (!f) {
        perror("Unable to open input file");
        return NULL;
    }


    //dist_matrix * = ;
}
