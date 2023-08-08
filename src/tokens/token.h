#pragma once

#include "token_type.h"

#include <stdbool.h>

typedef struct token_t {
    token_type_t type;
    void *data;
    int line;
} token_t;

token_t token_new(token_type_t type, int line);

token_t token_string(const char *value, int line);
token_t token_number(double value, int line);
token_t token_boolean(bool value, int line);
token_t token_symbol(const char *value, int line);

void token_free(token_t *token);