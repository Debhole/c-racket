#include "definition_book.h"

definition_book_t definition_book_new(void) {
    unsigned int capacity = 8;
    definition_t *definitions = malloc(sizeof(definition_t) * capacity);

    return (definition_book_t){
            .len = 0,
            .capacity = capacity,
            .definitions = definitions,
    };
}

void definition_book_free(definition_book_t *book) {
    for (unsigned int i = 0; i < book->len; i += 1) {
        definition_free(&book->definitions[i]);
    }

    free(book->definitions);
    book->len = 0;
    book->capacity = 0;
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
    }

    book->definitions[book->len] = def;
    book->len += 1;

    return true;
}

bool definition_book_contains(definition_book_t *book, const char *name, definition_t *def) {
    for (unsigned int i = 0; i < book->len; i += 1) {
        if (strcmp(book->definitions[i].name, name) == 0) {
            if (def) {
                *def = book->definitions[i];
            }
            return true;
        }
    }

    return false;
}
