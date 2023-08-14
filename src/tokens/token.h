#pragma once

#include "number.h"
#include "token_type.h"

#include <stdbool.h>

typedef struct token_t {
    token_type_t type;
    void *data;
    unsigned int line;
} token_t;

token_t token_new(token_type_t type, unsigned int line);
void token_free(token_t *token);

token_t token_string(const char *value, unsigned int line);
token_t token_number(double value, unsigned int line);
token_t token_boolean(bool value, unsigned int line);
token_t token_rational(rational_number_t num, unsigned int line);
token_t token_real(real_number_t num, unsigned int line);
token_t token_symbol(const char *name, unsigned int line);
token_t token_keyword(const char *name, unsigned int line);
token_t token_error(const char *value, unsigned int line);

void token_print(const token_t *t);