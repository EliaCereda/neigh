#ifndef NEIGH_DIST_MATRIX_H
#define NEIGH_DIST_MATRIX_H

#include <stdint.h>
#include <assert.h>

struct dist_matrix {
    /* Number of species */
    uint32_t species_count;
  
    /* Names of the species */
    char **species_names;

    /* Distance matrix */
    double distances[];
};

typedef struct dist_matrix dist_matrix;

dist_matrix *dist_matrix_init(uint32_t species_count);
void dist_matrix_free(dist_matrix *dmat);

/* Extract the distance between species c1 and c2 */
static double *dist_matrix_element(dist_matrix *dmat, uint32_t c1, uint32_t c2) {
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

/* Get the matrix size for a given number of species */
static uint32_t dist_matrix_size(uint32_t species_count) {
    return species_count * (species_count - 1) / 2;
}

/* Print the distance matrix */
void dist_matrix_print(const dist_matrix *dmat);

#endif //NEIGH_DIST_MATRIX
