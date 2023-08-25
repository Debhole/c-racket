#pragma once

#include "ast_tag.h"
#include "tokens/token_list.h"

typedef struct ast_node_t {
    ast_tag_t tag;
    void *data;
    size_t data_size;
    struct ast_node_t **children;
    unsigned int num_children;
} ast_node_t;

ast_node_t *ast_node_new(ast_tag_t tag, void *data, size_t data_size, unsigned int num_children, ast_node_t **children);
void ast_node_free(ast_node_t *node);

ast_node_t *ast_node_clone(ast_node_t *node);

void ast_node_print(ast_node_t *node);
