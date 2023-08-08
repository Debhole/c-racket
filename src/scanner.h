#pragma once

#include "tokens/token_list.h"

#include <stdbool.h>

typedef struct scanner_t {
    char *source;
    size_t len;
    int loc;
    int line;
} scanner_t;

scanner_t scanner_new(const char *source);
void scanner_free(scanner_t *s);

token_list_t scanner_get_tokens(scanner_t *s);
token_t scanner_next_token(scanner_t *s);

char scanner_peek(scanner_t *s);
char scanner_advance(scanner_t *s);
char scanner_advance_line(scanner_t *s);
bool scanner_consume_whitespace(scanner_t *s);

bool scanner_has_next(scanner_t *s);
