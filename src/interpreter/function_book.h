#pragma once

#include "function.h"

typedef struct function_book_t {
    unsigned int len;
    unsigned int capacity;
    primitive_function_t *pfns;
} function_book_t;

function_book_t function_book_new(void);
void function_book_free(function_book_t *functions);

bool function_book_push(function_book_t *functions, primitive_function_t pfn);
bool function_book_contains(function_book_t *functions, const char *name, primitive_function_t *pfn);
