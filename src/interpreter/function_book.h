#pragma once

#include "function.h"

typedef struct function_book_t {
    unsigned int plen;
    unsigned int pcapacity;
    primitive_function_t *pfns;

    unsigned int clen;
    unsigned int ccapacity;
    custom_function_t  *cfns;
} function_book_t;

function_book_t function_book_new(void);
void function_book_free(function_book_t *functions);

bool function_book_push_primitive(function_book_t *functions, primitive_function_t pfn);
bool function_book_contains_primitive(function_book_t *functions, const char *name, primitive_function_t *pfn);

bool function_book_push_custom(function_book_t *functions, custom_function_t cfn);
bool function_book_contains_custom(function_book_t *functions, const char *name, custom_function_t *cfn);
