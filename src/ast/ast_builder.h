#pragma once

#include "ast.h"
#include "ast_list.h"
#include "tokens/token_list.h"

typedef struct ast_builder_t {
    token_list_t *tokens;
    unsigned int loc;
} ast_builder_t;

ast_builder_t ast_builder_new(token_list_t *tokens);
void ast_builder_free(ast_builder_t *b);

ast_list_t ast_builder_get_trees(ast_builder_t *b);

ast_node_t *ast_builder_next_tree(ast_builder_t *b);

ast_node_t *ast_builder_next_expression(ast_builder_t *b, token_type_t end);

bool ast_builder_advance(ast_builder_t *b, token_t *t);

bool ast_builder_peek(ast_builder_t *b, token_t *t);
