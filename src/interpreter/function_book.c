#include "function_book.h"

function_book_t function_book_new(void) {
    int capacity = 8;
    primitive_function_t *pfns = malloc(sizeof(primitive_function_t) * capacity);

    return (function_book_t) {
        .len = 0,
        .capacity = capacity,
        .pfns = pfns,
    };
}

void function_book_free(function_book_t *functions) {
    for (unsigned int i = 0; i < functions->len; i += 1) {
        primitive_function_free(&functions->pfns[i]);
    }

    free(functions->pfns);
    functions->pfns = NULL;
    functions->len = 0;
    functions->capacity = 0;
}

bool function_book_push(function_book_t *functions, primitive_function_t pfn) {
    if (functions->len >= functions->capacity) {
        functions->capacity *= 2;
        primitive_function_t *tmp_pfns = realloc(functions->pfns, sizeof(primitive_function_t) * functions->capacity);

        if (tmp_pfns) {
            functions->pfns = tmp_pfns;
        } else {
            return false;
        }
    }

    functions->pfns[functions->len] = pfn;
    functions->len += 1;

    return true;
}

bool function_book_contains(function_book_t *functions, const char *name, primitive_function_t *pfn) {
    for (unsigned int i = 0; i < functions->len; i += 1) {
        if (strcmp(functions->pfns[i].name, name) == 0) {
            *pfn = functions->pfns[i];
            return true;
        }
    }

    return false;
}
