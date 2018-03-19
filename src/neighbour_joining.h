#ifndef NEIGH_NEIGHBOUR_JOINING_H
#define NEIGH_NEIGHBOUR_JOINING_H

#include "dist_matrix.h"
#include "phylogenetic_tree.h"

/* Create a new dist_matrix, obtained joining the two nearest clusters. */
dist_matrix *nj_join_nearest_clusters(const dist_matrix *dmat, const char *new_name, uint32_t *c1, uint32_t *c2);

btree_storage *nj_tree_init(const dist_matrix *dmat, btree_node **leafs);
void nj_tree_add_node(const dist_matrix *dmat, btree_storage *storage, btree_node **working_nodes, const char *name, uint32_t c1, uint32_t c2);

#endif //NEIGH_NEIGHBOUR_JOINING_H
