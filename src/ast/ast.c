#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

ast_node_t *ast_node_new(ast_tag_t tag, void *data, size_t data_size, unsigned int num_children, ast_node_t **children) {
    ast_node_t *node = malloc(sizeof(ast_node_t));
    node->tag = tag;
    node->num_children = num_children;
    node->children = children;

    node->data_size = data_size;
    node->data = malloc(data_size);
    memcpy(node->data, data, data_size);

    return node;
}

void ast_node_free(ast_node_t *node) {
    for (unsigned int i = 0; i < node->num_children; i += 1) {
        ast_node_free(node->children[i]);
    }

    free(node->data);
    free(node->children);
    free(node);
}

ast_node_t *ast_node_clone(ast_node_t *node) {
    ast_node_t *clone = malloc(sizeof(ast_node_t));
    clone->tag = node->tag;
    clone->num_children = node->num_children;

    clone->data_size = node->data_size;
    clone->data = malloc(clone->data_size);
    memcpy(clone->data, node->data, clone->data_size);

    clone->num_children = node->num_children;
    clone->children = malloc(sizeof(ast_node_t *) * clone->num_children);
    for (unsigned int i = 0; i < clone->num_children; i += 1) {
        clone->children[i] = ast_node_clone(node->children[i]);
    }

    return clone;
}
