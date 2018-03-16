#ifndef NEIGH_DIST_MATRIX_H
#define NEIGH_DIST_MATRIX_H

#include <stdint.h>

struct dist_matrix {
  /* Number of species */
  uint32_t species_count;
  
  /* Names of the species */
  char **species_names;

  /* Distance matrix */
  double distances[];
};

typedef struct dist_matrix dist_matrix;

dist_matrix *dist_matrix_init();
void dist_matrix_free(dist_matrix *dmat);

#endif //NEIGH_DIST_MATRIX
