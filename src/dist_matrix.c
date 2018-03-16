#include "dist_matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

dist_matrix *dist_matrix_init(uint32_t species_count) {
    dist_matrix *dmat;

    size_t matrix_size = dist_matrix_size(species_count);
    size_t struct_size = sizeof(dist_matrix) + matrix_size * sizeof(dmat->distances[0]);

    dmat = malloc(struct_size);

    if (dmat != NULL) {
        dmat->species_count = species_count;
        dmat->species_names = calloc(species_count, sizeof(dmat->species_names[0]));

        if (dmat->species_names == NULL) {
            dist_matrix_free(dmat);

            return NULL;
        }
    }

    return dmat;
}

void dist_matrix_free(dist_matrix *dmat) {
    for (uint32_t i = 0; i < dmat->species_count; i++) {
        free(dmat->species_names[i]);
    }

    free(dmat->species_names);
    free(dmat);
}

double *dist_matrix_element(dist_matrix *dmat, uint32_t c1, uint32_t c2) {
    assert(c1 != c2);
    assert(c1 >= 0 && c1 < dmat->species_count);
    assert(c2 >= 0 && c2 < dmat->species_count);

    if (c1 < c2) {
        return dist_matrix_element(dmat, c2, c1);
    } else {
        uint32_t idx = (c1 * (c1 - 1) / 2) + c2;
        return dmat->distances + idx;
    }
}

uint32_t dist_matrix_size(uint32_t species_count) {
    return species_count * (species_count - 1) / 2;
}

void dist_matrix_print(const dist_matrix *dmat) {

    size_t max_length = 6;

    for (uint32_t i = 0; i < dmat->species_count; i++) {
        size_t length = strlen(dmat->species_names[i]);

        if (length > max_length) {
            max_length = length;
        }
    }

    printf("%*s\t", max_length, "");

    for (uint32_t i = 0; i < dmat->species_count; i++) {
        printf("%*s\t", max_length, dmat->species_names[i]);
    }

    printf("\n");

    for (uint32_t i = 0; i < dmat->species_count; i++) {
        printf("%*s\t", max_length, dmat->species_names[i]);

        for (uint32_t j = 0; j < i; j++) {
            printf("%*.4lf\t", max_length, *dist_matrix_element(dmat, i, j));
        }

        printf("%*s\n", max_length, "-");
    }


}
