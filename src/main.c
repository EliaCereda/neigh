#include "config.h"
#include "io.h"
#include "neighbour_joining.h"
#include "viz.h"

#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <assert.h>

int main(int argc, char *argv[]) {
    const char *files[] = {
            //"examples/hello_world.in",
            //"examples/evolution.in",
            //"examples/wiki.in",
            "examples/INGI2368.in",
    };

    int n = sizeof(files) / sizeof(*files);

    for (int i = 0; i < n; i++) {
        printf("Processing file '%s'...\n\n", files[i]);

        dist_matrix *dmat = load_file(files[i]);

        if (!dmat) {
            /* Error, stop here */
            continue;
        }

        double u[dmat->species_count];

        /* Ensure that cluster_name has enough space for the longest possible name */
        char cluster_name[2 + 3 * sizeof(dmat->species_count)];
        uint32_t cluster_id = 1;

        btree_node *partial_trees[dmat->species_count];
        btree_storage *tree_storage = nj_tree_init(dmat, partial_trees);

        while (dmat->species_count >= 2) {
            dist_matrix_print(dmat);
            printf("\n");

            btree_print_trees(partial_trees, dmat->species_count);
            printf("\n");

#if HAS_GRAPHVIZ
            viz_visualize_trees(partial_trees, dmat->species_count, "prova.part.pdf", "pdf");
#endif
            
            /* Compute the average distance of each clusters from the others */
            dist_matrix_compute_avg_distances(dmat, u);

            /* Find the pair of nearest clusters */
            uint32_t c1, c2;
            nj_find_nearest_clusters(dmat, u, &c1, &c2);

            /* Generate a name for the new cluster */
            unsigned long result = snprintf(cluster_name, sizeof(cluster_name), "C_%" PRIu32, cluster_id);
            assert(result > 0 && result < sizeof(cluster_name));

            printf("Joining clusters '%s' and '%s' in '%s'.\n\n", dmat->species_names[c1], dmat->species_names[c2], cluster_name);

            /* Add a node for the new cluster to the array of partial trees */
            nj_tree_add_node(dmat, u, tree_storage, partial_trees, cluster_name, c1, c2);

            /* Create a new dist_matrix joining the specified clusters */
            dist_matrix *joined = nj_join_clusters(dmat, cluster_name, c1, c2);
            
            if (joined == NULL) {
                /* Error, stop here */
                break;
            }

            /* Release the old distance matrix */
            dist_matrix_free(dmat);
            dmat = joined;

            cluster_id++;
        }

        btree_node *phyl_tree = partial_trees[0];

        btree_print_tree(phyl_tree);
        printf("\n\n");

#if HAS_GRAPHVIZ
        viz_visualize_tree(phyl_tree, "prova.pdf", "pdf");
#endif

        dist_matrix_free(dmat);
        btree_storage_free(tree_storage);
    }

    return 0;
}
