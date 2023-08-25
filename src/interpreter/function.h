#pragma once

#include "ast/ast.h"
#include "ast/ast_list.h"
#include <stdbool.h>
typedef struct primitive_function_t {
    char *name;
    unsigned int arity;
    bool precise_arity;
    ast_node_t *(*fn)(ast_list_t *args);
} primitive_function_t;

primitive_function_t primitive_function_new(
        const char *name,
        unsigned int arity,
        bool precise_arity,
        ast_node_t *(*fn)(ast_list_t *args));
void primitive_function_free(primitive_function_t *fn);

ast_node_t *prim_isboolean(ast_list_t *args);
ast_node_t *prim_isnumber(ast_list_t *args);
ast_node_t *prim_issymbol(ast_list_t *args);
ast_node_t *prim_isstring(ast_list_t *args);

ast_node_t *prim_add(ast_list_t *args);
ast_node_t *prim_sub(ast_list_t *args);
ast_node_t *prim_mul(ast_list_t *args);

ast_node_t *prim_string_append(ast_list_t *args);

typedef struct custom_function_t {
    char *name;
    ast_list_t args;
    bool precise_arity;
    ast_node_t *definition;
} custom_function_t;

custom_function_t custom_function_new(
        const char *name,
        ast_list_t *args,
        bool precise_arity,
        ast_node_t *definition
        );
void custom_function_free(custom_function_t *fn);
