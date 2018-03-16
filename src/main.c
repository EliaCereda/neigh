#include "io.h"

int main(int argc, char *argv[]) {
    dist_matrix *input = load_file("examples/hello_world.in");
    
    input = load_file("examples/evolution.in");
    
    if (input) {
        dist_matrix_print(input);
    }
    
    return 0;
}
