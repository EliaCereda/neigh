#include "io.h"
#include <stdio.h>
#include <memory.h>
#include <ctype.h>

size_t trim_trailing_space(char *s) {
    size_t length = strlen(s);

    while (length > 0 && isspace(s[length - 1])) {
        length--;
    }

    s[length] = '\0';

    return length;
}

dist_matrix *load_file(const char *file_name) {
    FILE *f = fopen(file_name, "r");

    if (!f) {
        perror("Unable to open input file");
        return NULL;
    }


    //dist_matrix * = ;
}
