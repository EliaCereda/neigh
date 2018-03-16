#include "io.h"
#include <stdio.h>
#include <memory.h>
#include <inttypes.h>
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

    int result;
    uint32_t species_count;

    result = fscanf(f, "%" SCNu32, &species_count);

    if (result != 1) {
        perror("Invalid species count");

        fclose(f);
        return NULL;
    }

    for (uint32_t i = 0; i < species_count; i++) {
        /* species name: up to 30 alphabetic or whitespace characters */
        char species_name[31];

        result = fscanf(f, " %30[^0-9\n]", species_name);

        if (result != 1) {
            perror("Invalid species name");

            fclose(f);
            return NULL;
        }

        trim_trailing_space(species_name);
    }

    fclose(f);

    dist_matrix *dmat = NULL;

    return dmat;
}
