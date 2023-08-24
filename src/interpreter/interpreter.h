#pragma once

#include "ast/ast_list.h"
#include "function_book.h"

typedef struct interpreter_t {
    function_book_t function_book;
} interpreter_t;

interpreter_t interpreter_new(void);
void interpreter_free(interpreter_t *interpreter);

ast_list_t interpreter_eval(interpreter_t *interpreter, const char *source);
ast_node_t *interpreter_eval_node(interpreter_t *interpreter, ast_node_t *node);
ast_node_t *interpreter_eval_fn(interpreter_t *interpreter, const char *fn, ast_node_t **args, unsigned int num_args);
