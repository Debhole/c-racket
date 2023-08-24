#pragma once

#include <string.h>

typedef enum token_type_t {
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,

    TOKEN_QUOTE,
    TOKEN_QUASIQUOTE,
    TOKEN_UNQUOTE,

    TOKEN_STRING,
    TOKEN_INTEGER,
    TOKEN_DOUBLE,
    TOKEN_BOOLEAN,
    TOKEN_SYMBOL,
    TOKEN_KEYWORD,

    TOKEN_EOF,
    TOKEN_ERROR,
} token_type_t;

const char *token_type_to_string(token_type_t type);
