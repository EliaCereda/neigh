#include "phylogenetic_tree.h"
#include "utilities.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

size_t btree_storage_size(uint32_t nodes_count) {
    return sizeof(btree_storage) + (nodes_count * member_size(btree_storage, nodes[0]));
}

btree_storage *btree_storage_init(uint32_t nodes_count) {
    size_t size = btree_storage_size(nodes_count);

    /* Zero-initialize the storage to ensure that btree_storage_free can correctly
     * free the strings */
    btree_storage *storage = calloc(size, 1);

    if (storage != NULL) {
        storage->nodes_count = nodes_count;
    }
    
    return storage;
}

void btree_storage_free(btree_storage *storage) {
    if (storage) {
        for (uint32_t i = 0; i < storage->nodes_count; ++i) {
            free(storage->nodes[i].node_name);
        }

        free(storage);
    }
}

btree_node *btree_storage_fetch(btree_storage *storage) {
    assert(storage->used_nodes < storage->nodes_count);
    
    uint32_t idx = storage->used_nodes;
    btree_node *node = &storage->nodes[idx];
    
    storage->used_nodes++;
    
    return node;
}

static void _btree_print(btree_node *root, uint32_t depth, double distance) {
    if (root == NULL) {
        return;
    }

    _btree_print(root->left, depth + 1, root->distance_left);

    if (depth > 0) {
        for (uint32_t i = 1; i < depth; i++) {
            printf("  |        ");
        }
    
        printf("  |--%.2lf-- ", distance);
    }
    
    printf("%s\n", root->node_name);

    _btree_print(root->right, depth + 1, root->distance_right);
}

void btree_print(btree_node *root) {
    _btree_print(root, 0, 0);
}
