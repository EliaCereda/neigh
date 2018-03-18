#include "dist_matrix.h"

#include <assert.h>
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
    if (dmat) {
        if (dmat->species_names) {
            for (uint32_t i = 0; i < dmat->species_count; i++) {
                free(dmat->species_names[i]);
            }

            free(dmat->species_names);
        }

        free(dmat);
    }
}

static uint32_t dist_matrix_get_offset(const dist_matrix *dmat, uint32_t s1, uint32_t s2) {
    assert(s1 != s2);
    assert(s1 >= 0 && s1 < dmat->species_count);
    assert(s2 >= 0 && s2 < dmat->species_count);

    if (s1 < s2) {
        return dist_matrix_get_offset(dmat, s2, s1);
    } else {
        return (s1 * (s1 - 1) / 2) + s2;
    }
}

double *dist_matrix_element(dist_matrix *dmat, uint32_t s1, uint32_t s2) {
    return dmat->distances + dist_matrix_get_offset(dmat, s1, s2);
}

double dist_matrix_distance(const dist_matrix *dmat, uint32_t s1, uint32_t s2) {
    return *(dmat->distances + dist_matrix_get_offset(dmat, s1, s2));
}

double dist_matrix_distance_from_others(const dist_matrix *dmat, uint32_t s) {
    assert(dmat->species_count > 2);
    
    double distance = 0;
    
    for (uint32_t i = 0; i < dmat->species_count; i++) {
        if (i == s) {
            continue;
        }
        
        distance += dist_matrix_distance(dmat, s, i);
    }
    
    return distance / (dmat->species_count - 2);
}

uint32_t dist_matrix_size(uint32_t species_count) {
    return species_count * (species_count - 1) / 2;
}

void dist_matrix_print(const dist_matrix *dmat) {

    int max_length = 6;

    for (uint32_t i = 0; i < dmat->species_count; i++) {
        int length = (int)strlen(dmat->species_names[i]);

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
            printf("%*.4lf\t", max_length, dist_matrix_distance(dmat, i, j));
        }

        printf("%*s\n", max_length, "*");
    }


}
