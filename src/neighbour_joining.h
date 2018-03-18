#ifndef NEIGH_NEIGHBOUR_JOINING_H
#define NEIGH_NEIGHBOUR_JOINING_H

#include "dist_matrix.h"

/* Create a new dist_matrix, obtained joining the two nearest clusters. */
dist_matrix *nj_join_nearest_clusters(const dist_matrix *dmat);

#endif //NEIGH_NEIGHBOUR_JOINING_H
