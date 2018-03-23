#include "config.h"

#include "io.h"
#include "neighbour_joining.h"
#include "viz.h"

#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <assert.h>

//#include <iostream>
// for arguments options in command line
#include "stdlib.h"
#include "cmdline.h"

/*void perform_stuff(const char *input_filename, const char *output_filename) {
    printf("Reading from input file %s\n", input_filename);
    printf("Writing to output file %s\n", output_filename);
}
*/

int main(int argc, char *argv[]) {
    
    struct gengetopt_args_info args_info;
    if (cmdline_parser(argc, argv, &args_info) != 0) {
        exit(1);
    }
    
    if (args_info.inputs_num != 1) {
        fprintf(stderr, "Program requires filename argument\n");
        return EXIT_FAILURE;
    }
    
    const char *file = args_info.inputs[0];
            //{ "examples/hello_world.in",
            //"examples/evolution.in",
            //"examples/wiki.in",
            //"examples/INGI2368.in",};
    
    
    /* Print arguments from command line */
    int i;
    // printf("argc=%d\n", argc);
    for(i = 0; i < argc; i++) {
        if (i == 1) {
             printf("Processing file '%s'...\n\n", file);
        } else {
            printf("argv[%d]:=%s\n", i, argv[i]);
        }
    }
    
    /*char *output_filename;
    if (args_info.output_given) {
        output_filename = args_info.output_arg;
    } else {
        output_filename = "output.fits";
    }*/
    
   
    dist_matrix *dmat = load_file(file);
    
    if (!dmat) {
        // Error, stop here
        return 1;
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

        
        btree_node *phyl_tree = partial_trees[0];
        
        btree_print_tree(phyl_tree);
        printf("\n\n");
        
#if HAS_GRAPHVIZ
        viz_visualize_tree(phyl_tree, "prova.pdf", "pdf");
#endif
        
        dist_matrix_free(dmat);
        btree_storage_free(tree_storage);
    }

    //perform_stuff(file, output_filename);
    
    return EXIT_SUCCESS;
}
