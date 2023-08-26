#include "definition_book.h"

definition_book_t definition_book_new(void) {
    unsigned int capacity = 8;
    definition_t *definitions = malloc(sizeof(definition_t) * capacity);
    unsigned int *scopes = malloc(sizeof(unsigned int) * capacity);

    return (definition_book_t){
            .len = 0,
            .capacity = capacity,
            .scope_idx = 0,
            .scopes = scopes,
            .definitions = definitions,
    };
}

void definition_book_free(definition_book_t *book) {
    for (unsigned int i = 0; i < book->len; i += 1) {
        definition_free(&book->definitions[i]);
    }

    free(book->definitions);
    free(book->scopes);
    book->len = 0;
    book->capacity = 0;
    book->scope_idx = 0;
}

bool definition_book_push(definition_book_t *book, definition_t def) {
    if (book->len >= book->capacity) {
        book->capacity *= 2;
        definition_t *tmp_dfns = realloc(book->definitions, sizeof(definition_t) * book->capacity);

        if (tmp_dfns) {
            book->definitions = tmp_dfns;
        } else {
            return false;
        }

        unsigned int *tmp_scopes = realloc(book->scopes, sizeof(unsigned int) * book->capacity);

        if (tmp_scopes) {
            book->scopes = tmp_scopes;
        } else {
            return false;
        }
    }

    book->definitions[book->len] = def;
    book->scopes[book->len] = book->scope_idx;
    book->len += 1;

    return true;
}

bool definition_book_contains(definition_book_t *book, const char *name, definition_t *def) {
    for (unsigned int i = 0; i < book->len; i += 1) {
        unsigned int reverse = book->len - i - 1;
        if (strcmp(book->definitions[reverse].name, name) == 0) {
            if (def) {
                *def = book->definitions[reverse];
            }
            return true;
        }
    }

    return false;
}

void definition_book_begin_scope(definition_book_t *book) {
    book->scope_idx += 1;
}

void definition_book_end_scope(definition_book_t *book) {
    unsigned int scope_len = book->len;
    for (unsigned int i = 0; i < scope_len; i += 1) {
        unsigned int reverse = scope_len - i - 1;
        if (book->scopes[reverse] >= book->scope_idx) {
            definition_free(&book->definitions[reverse]);
            book->len -= 1;
        }
    }

    book->scope_idx -= 1;
}
