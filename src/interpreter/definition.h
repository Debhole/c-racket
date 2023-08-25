#pragma once

#include "ast/ast.h"

typedef struct definition_t {
    char *name;
    ast_node_t *value;
} definition_t;

definition_t definition_new(const char *name, ast_node_t *value);
void definition_free(definition_t *definition);
