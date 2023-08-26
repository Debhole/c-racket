#pragma once

#include "ast/ast_list.h"
#include "definition_book.h"
#include "function_book.h"

typedef struct interpreter_t {
    function_book_t function_book;
    definition_book_t definition_book;
} interpreter_t;

interpreter_t interpreter_new(void);
void interpreter_free(interpreter_t *interpreter);

ast_list_t interpreter_eval(interpreter_t *interpreter, const char *source);
ast_node_t *interpreter_eval_node(interpreter_t *interpreter, ast_node_t *node);
ast_node_t *interpreter_eval_fn(interpreter_t *interpreter, const char *fn, ast_node_t **args, unsigned int num_args);
ast_node_t *interpreter_eval_name(interpreter_t *interpreter, const char *name);

ast_node_t *interpreter_eval_kw(interpreter_t *interpreter, const char *name, ast_node_t **args, unsigned int num_args);

ast_node_t *interpreter_eval_if(interpreter_t *interpreter, ast_node_t **args, unsigned int num_args);
ast_node_t *interpreter_eval_and(interpreter_t *interpreter, ast_node_t **args, unsigned int num_args);
ast_node_t *interpreter_eval_or(interpreter_t *interpreter, ast_node_t **args, unsigned int num_args);

bool interpreter_eval_definition(interpreter_t *interpreter, ast_node_t *node);
