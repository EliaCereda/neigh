#include "neighbour_joining.h"

#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

double nj_find_nearest_clusters(const dist_matrix *dmat, uint32_t *c1, uint32_t *c2) {
    assert(dmat->species_count >= 2);
    assert(c1 != NULL);
    assert(c2 != NULL);

    double u[dmat->species_count];

    /* Compute the average distance of each clusters from the others */
    dist_matrix_compute_avg_distances(dmat, u);

    /*
     * Find the two clusters that minimize the distance metric:
     *      min { D(c1, c2) - U(c1) - U(c2) } among all c1, c2 in dmat
     */
    double min_distance = INFINITY;

    for (uint32_t i = 0; i < dmat->species_count; i++) {
        for (uint32_t j = 0; j < i; j++) {
            double distance = dist_matrix_distance(dmat, i, j) - u[i] - u[j];

            if (distance < min_distance) {
                min_distance = distance;

                *c1 = i;
                *c2 = j;
            }
        }
    }

    /* A pair of clusters should always be found */
    assert(isfinite(min_distance));
    
    return min_distance;
}

dist_matrix *nj_join_clusters(const dist_matrix *dmat, const char *new_name, uint32_t c1, uint32_t c2) {
    assert(dmat->species_count >= 2);

    dist_matrix *out = dist_matrix_init(dmat->species_count - 1);

    if (out == NULL) {
        perror("Unable to create output distance matrix");
        return NULL;
    }
    
    uint32_t k = 0;

    for (uint32_t i = 0; i < dmat->species_count; i++) {
        const char *species_name;
        uint32_t cluster_size;

        if (i == c1) {
            /* Replace c1 with the union of clusters c1 and c2 */
            species_name = new_name;
            cluster_size = dmat->cluster_sizes[c1] + dmat->cluster_sizes[c2];
        } else if (i == c2) {
            /* Remove cluster c2 */
            continue;
        } else {
            /* Leave other clusters unchanged */
            species_name = dmat->species_names[i];
            cluster_size = dmat->cluster_sizes[i];
        }

        out->species_names[k] = strdup(species_name);
        out->cluster_sizes[k] = cluster_size;

        /* Compute the distances */
        uint32_t l = 0;

        for (uint32_t j = 0; j < i; j++) {
            if (j == c2) {
                /* Remove cluster c2 */
                continue;
            }

            double distance;

            if (i == c1) {
                uint32_t s1 = dmat->cluster_sizes[c1];
                uint32_t s2 = dmat->cluster_sizes[c2];
                uint32_t sj = dmat->cluster_sizes[j];

                double d1 = dist_matrix_distance(dmat, c1, j) * (s1 * sj);
                double d2 = dist_matrix_distance(dmat, c2, j) * (s2 * sj);

                distance = (d1 + d2) / ((s1 + s2) * sj);
            } else if (j == c1) {
                uint32_t si = dmat->cluster_sizes[i];
                uint32_t s1 = dmat->cluster_sizes[c1];
                uint32_t s2 = dmat->cluster_sizes[c2];

                double d1 = dist_matrix_distance(dmat, i, c1) * (si * s1);
                double d2 = dist_matrix_distance(dmat, i, c2) * (si * s2);

                distance = (d1 + d2) / (si * (s1 + s2));
            } else {
                distance = dist_matrix_distance(dmat, i, j);
            }

            *(dist_matrix_element(out, k, l)) = distance;

            l++;
        }

        k++;
    }

    return out;
}

btree_storage *nj_tree_init(const dist_matrix *dmat, btree_node **leafs) {
    uint32_t node_count = 2 * dmat->species_count - 1;
    btree_storage *storage = btree_storage_init(node_count);
    
    for (uint32_t i = 0; i < dmat->species_count; i++) {
        leafs[i] = btree_storage_fetch(storage);
        leafs[i]->node_name = strdup(dmat->species_names[i]);
    }
    
    return storage;
}

void nj_tree_add_node(const dist_matrix *dmat, btree_storage *storage, btree_node **working_nodes, const char *name, uint32_t c1, uint32_t c2) {
    btree_node *node = btree_storage_fetch(storage);

    node->node_name = strdup(name);
    node->left = working_nodes[c1];
    node->right = working_nodes[c2];

    working_nodes[c1] = node;

    for (uint32_t i = c2 + 1; i < dmat->species_count; i++) {
        working_nodes[i - 1] = working_nodes[i];
    }
}
