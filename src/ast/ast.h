#pragma once

#include "tokens/token_list.h"

typedef enum tag_t {
    TAG_EXPRESSION,
    TAG_DEFINITION,
    TAG_RATIONAL,
    TAG_REAL,
    TAG_STRING,
    TAG_BOOLEAN,
    TAG_KEYWORD,
    TAG_SYMBOL,
    TAG_COND_CLAUSE,
} tag_t;

typedef struct ast_node_t {
    tag_t tag;
    void *data;
    unsigned int num_children;
    struct ast_node_t **children;
} ast_node_t;

ast_node_t *ast_node_new(tag_t tag, void *data, unsigned int num_children, ast_node_t **children);
void ast_node_free_all(ast_node_t *node);