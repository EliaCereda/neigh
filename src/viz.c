//
// Created by Elia Cereda on 20/03/18.
//

#include "viz.h"

#include <graphviz/gvc.h>
#include <stdbool.h>

Agnode_t *viz_process_tree(const btree_node *root, Agraph_t *graph) {
    Agnode_t *node = agnode(graph, root->node_name, true);

    if (root->left != NULL) {
        Agnode_t *left = viz_process_tree(root->left, graph);

        agedge(graph, node, left, NULL, true);
    }

    if (root->right != NULL) {
        Agnode_t *right = viz_process_tree(root->right, graph);

        agedge(graph, node, right, NULL, true);
    }

    if (root->left == NULL && root->right == NULL) {
        // Leaf nodes
        agsafeset(node, "fontname", "Times-Bold", "");
    }

    return node;
}

void viz_visualize_tree(const btree_node *root, const char *output_file, const char *format) {
    /* set up a graphviz context */
    GVC_t *gvc = gvContext();

    /* Create a simple digraph */
    Agraph_t *graph = agopen("g", Agstrictundirected, NULL);

    viz_process_tree(root, graph);

    /* Compute a layout using layout engine from command line args */
    gvLayout(gvc, graph, "dot");

    /* Write the graph according to -T and -o options */
    gvRenderFilename(gvc, graph, format, output_file);

    /* Free layout data */
    gvFreeLayout(gvc, graph);

    /* Free graph structures */
    agclose(graph);

    gvFreeContext(gvc);
}
