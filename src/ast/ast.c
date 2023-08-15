#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

ast_node_t *ast_node_new(tag_t tag, void *data, unsigned int num_children, ast_node_t **children) {
    ast_node_t *node = malloc(sizeof(ast_node_t));
    node->tag = tag;
    node->data = data;
    node->num_children = num_children;
    node->children = children;

    return node;
}

void ast_node_free(ast_node_t *node) {
    for (int i = 0; i < node->num_children; i += 1) {
        ast_node_free(node->children[i]);
    }

    free(node);
}