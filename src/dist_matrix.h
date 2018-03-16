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
double *dist_matrix_element(dist_matrix *dmat, uint32_t c1, uint32_t c2);

/* Get the matrix size for a given number of species */
uint32_t dist_matrix_size(uint32_t species_count);

/* Print the distance matrix */
void dist_matrix_print(const dist_matrix *dmat);

#endif //NEIGH_DIST_MATRIX
