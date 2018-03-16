#include "dist_matrix.h"
#include <stdlib.h>

dist_matrix *dist_matrix_init() {
    dist_matrix *dmat = malloc(sizeof(dist_matrix));

    return dmat;
}

void dist_matrix_free(dist_matrix *dmat) {
    free(dmat);
}
