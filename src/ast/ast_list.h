#pragma once

#include "ast.h"

typedef struct ast_list_t {
    unsigned int len;
    unsigned int capacity;
    ast_node_t **trees;
} ast_list_t;

ast_list_t ast_list_new(void);
ast_list_t ast_list_empty(void);
void ast_list_free(ast_list_t *list);

void ast_list_deform(ast_list_t *list, ast_node_t ***raw_trees, unsigned int *num_children);

bool ast_list_push(ast_list_t *list, ast_node_t *tree);
bool ast_list_get(ast_list_t *list, unsigned int idx, ast_node_t **tree);
