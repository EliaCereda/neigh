#include "phylogenetic_tree.h"
#include <stdlib.h>
#include "utilities.h"

btree *btree_size(uint32_t nodes_count) {
    size_t struct_size = sizeof(btree) + (nodes_count * member_size(btree, nodes[0]));
    
    return struct_size;
}

btree *btree_init(uint32_t nodes_count) {
    btree *btree;
    btree = malloc(btree_size(nodes_count));

    return btree;
}

void btree_free(btree *btree) {
    if (btree) {
        free(btree);
    }
}

