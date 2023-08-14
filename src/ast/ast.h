#pragma once

#include "tokens/token_list.h"

typedef enum tag_t {
    TAG_EXPRESSION = (1u << 0),
    TAG_RATIONAL = (1u << 1),
    TAG_REAL = (1u << 2),
    TAG_STRING = (1u << 3),
    TAG_BOOLEAN = (1u << 4),
    TAG_KEYWORD = (1u << 5),
    TAG_SYMBOL = (1u << 6),
    TAG_COND_CLAUSE = (1u << 7),
} tag_t;

typedef struct ast_node_t {
    tag_t tag;
    void *data;
    unsigned int num_children;
    struct ast_node_t **children;
} ast_node_t;

ast_node_t *ast_node_new(tag_t tag, void *data, unsigned int num_children, ast_node_t **children);
void ast_node_free_all(ast_node_t *node);