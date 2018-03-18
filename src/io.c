#include "io.h"
#include <stdio.h>
#include <memory.h>
#include <inttypes.h>
#include <ctype.h>

static size_t trim_trailing_space(char *s) {
    size_t length = strlen(s);

    while (length > 0 && isspace(s[length - 1])) {
        length--;
    }

    s[length] = '\0';

    return length;
}

#define CHECK_SCANF_RESULT(result, value, message, file, dmat) \
    if (result != value) {                                     \
        perror(message);                                       \
                                                               \
        dist_matrix_free(dmat);                                \
        fclose(file);                                          \
                                                               \
        return NULL;                                           \
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

    CHECK_SCANF_RESULT(result, 1, "Invalid species count", f, NULL);

    dist_matrix *dmat = dist_matrix_init(species_count);

    if (!dmat) {
        perror("Unable to create distance matrix");
        return NULL;
    }

    for (uint32_t i = 0; i < species_count; i++) {
        /* species name: up to 30 alphabetic or whitespace characters */
        char species_name[31];

        result = fscanf(f, " %30[^0-9*\n]", species_name);

        CHECK_SCANF_RESULT(result, 1, "Invalid species name", f, dmat);

        trim_trailing_space(species_name);

        dmat->species_names[i] = strdup(species_name);
        dmat->cluster_sizes[i] = 1;

        for (uint32_t j = 0; j < i; j++) {
            double *element = dist_matrix_element(dmat, i, j);

            result = fscanf(f, "%lf", element);

            CHECK_SCANF_RESULT(result, 1, "Invalid distance", f, dmat);
        }

        /* Ignore asterisks at the end of the line */
        fscanf(f, " *");
    }

    fclose(f);

    return dmat;
}
