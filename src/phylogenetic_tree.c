#include "phylogenetic_tree.h"
#include "utilities.h"

#include <assert.h>
#include <stdlib.h>

size_t btree_storage_size(uint32_t nodes_count) {
    return sizeof(btree_storage) + (nodes_count * member_size(btree_storage, nodes[0]));
}

btree_storage *btree_storage_init(uint32_t nodes_count) {
    size_t size = btree_storage_size(nodes_count);
    btree_storage *storage = malloc(size);

    return storage;
}

void btree_storage_free(btree_storage *storage) {
    free(storage);
}

btree_node *btree_storage_fetch(btree_storage *storage) {
    assert(storage->used_nodes < storage->nodes_count);
    
    uint32_t idx = storage->used_nodes;
    btree_node *node = &storage->nodes[idx];
    
    storage->used_nodes++;
    
    return node;
}
