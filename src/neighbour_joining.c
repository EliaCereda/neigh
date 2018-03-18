#include "neighbour_joining.h"

#include <assert.h>
#include <math.h>
#include <string.h>

dist_matrix *nj_join_nearest_clusters(const dist_matrix *dmat) {
    assert(dmat->species_count >= 2);

    uint32_t c1;
    uint32_t c2;

    if (dmat->species_count == 2) {
        c1 = 0;
        c2 = 1;
    } else {
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
                    c1 = i;
                    c2 = j;
                }
            }
        }
    }

    dist_matrix *out = dist_matrix_init(dmat->species_count - 1);

    // FIXME: check out NULL
    
    uint32_t k = 0;

    for (uint32_t i = 0; i < dmat->species_count; i++) {
        const char *species_name;
        uint32_t cluster_size;

        if (i == c1) {
            /* Replace c1 with the union of clusters c1 and c2 */
            species_name = "C_overbar";
            cluster_size = dmat->cluster_sizes[c1] + dmat->cluster_sizes[c2];

            // TODO: compute distances
            uint32_t l = 0;
            
            for (uint32_t j = 0; j < dmat->species_count; j++) {
                if (j == c2) {
                    continue;
                }
                if (j != c1) {
                    double d1 = dist_matrix_distance(dmat, c1, j) / cluster_size;
                    double d2 = dist_matrix_distance(dmat, c2, j) / cluster_size;
                    
                    *(dist_matrix_element(out, k, l)) = d1 + d2;
                }
                
                l++;
            }
            
        } else if (i == c2) {
            /* Remove cluster c2 */
            continue;
        } else {
            /* Leave other clusters unchanged */
            species_name = dmat->species_names[i];
            cluster_size = dmat->cluster_sizes[i];

            uint32_t l = 0;

            for (uint32_t j = 0; j < i; j++) {
                if (j == c2) {
                    continue;
                }

                if (j != c1) {
                    *(dist_matrix_element(out, k, l)) = dist_matrix_distance(dmat, i, j);
                }

                l++;
            }
        }

        out->species_names[k] = strdup(species_name);
        out->cluster_sizes[k] = cluster_size;

        k++;
    }

    return out;
}
