#ifndef NEIGH_PHYLOGENETIC_TREE_H
#define NEIGH_PHYLOGENETIC_TREE_H

#include <stdio.h>
#include <stdint.h>

typedef struct btree_node btree_node;

struct btree_node {
    /* Left and right children */
    btree_node *left, *right;
    
    /* Name of the node */
    char *node_name;
};

typedef struct btree_storage btree_storage;

struct btree_storage {
    /* Number of nodes */
    uint32_t nodes_count;
    
    /* Number of used nodes / Index of first available node */
    uint32_t used_nodes;
    
    /* Array of tree nodes */
    btree_node nodes[];
};

size_t btree_storage_size(uint32_t nodes_count);
btree_storage *btree_storage_init(uint32_t nodes_count);
void btree_storage_free(btree_storage *tree);

/* Fetch the first available node */
btree_node *btree_storage_fetch(btree_storage *storage);

#endif /* NEIGH_PHYLOGENETIC_TREE_H */
