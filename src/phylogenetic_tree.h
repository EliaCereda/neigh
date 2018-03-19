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

typedef struct btree btree;

struct btree {
    /* Number of nodes */
    uint32_t nodes_count;
    
    /* Array of tree nodes */
    btree_node nodes[];
};

#endif /* NEIGH_PHYLOGENETIC_TREE_H */
