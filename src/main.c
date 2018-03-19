#include "io.h"
#include "neighbour_joining.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    char *files[] = {
            "examples/hello_world.in",
            "examples/evolution.in",
            "examples/wiki.in"
    };

    int n = sizeof(files) / sizeof(*files);

    for (int i = 0; i < n; i++) {
        dist_matrix *input = load_file(files[i]);

        if (!input) {
            continue;
        }

        dist_matrix_print(input);
        printf("\n");

        dist_matrix *joined = nj_join_nearest_clusters(input);

        dist_matrix_print(joined);
        printf("\n");
        
        dist_matrix_free(input);
        dist_matrix_free(joined);
    }

    return 0;
}
