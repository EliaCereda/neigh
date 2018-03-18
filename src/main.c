#include "io.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    char *files[] = {
            "examples/hello_world.in",
            "examples/evolution.in",
            "examples/wiki.in"
    };

    for (int i = 0; i < 3; i++) {
        dist_matrix *input = load_file(files[i]);

        if (input) {
            dist_matrix_print(input);
        }

        printf("\n");
    }

    return 0;
}
