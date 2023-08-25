#pragma once

#include "definition.h"

typedef struct definition_book_t {
    unsigned int len;
    unsigned int capacity;
    unsigned int scope_idx;
    unsigned int *scopes;
    definition_t *definitions;

} definition_book_t;

definition_book_t definition_book_new(void);
void definition_book_free(definition_book_t *book);

bool definition_book_push(definition_book_t *book, definition_t def);
bool definition_book_contains(definition_book_t *book, const char *name, definition_t *def);

void definition_book_begin_scope(definition_book_t *book);
void definition_book_end_scope(definition_book_t *book);
