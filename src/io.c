#include "io.h"
#include <stdio.h>
#include <memory.h>
#include <inttypes.h>
#include <ctype.h>

#define CHECK_SCANF_RESULT(result, value, message, file) \
    if (result != value) {                               \
        perror(message);                                 \
                                                         \
        fclose(file);                                    \
        return NULL;                                     \
    }

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

    CHECK_SCANF_RESULT(result, 1, "Invalid species count", f);

    for (uint32_t i = 0; i < species_count; i++) {
        /* species name: up to 30 alphabetic or whitespace characters */
        char species_name[31];

        result = fscanf(f, " %30[^0-9\n]", species_name);

        CHECK_SCANF_RESULT(result, 1, "Invalid species name", f);

        trim_trailing_space(species_name);
    }

    fclose(f);

    dist_matrix *dmat = NULL;

    return dmat;
}
