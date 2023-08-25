#include "function_book.h"

function_book_t function_book_new(void) {
    int pcapacity = 8;
    primitive_function_t *pfns = malloc(sizeof(primitive_function_t) * pcapacity);

    int ccapacity = 8;
    custom_function_t *cfns = malloc(sizeof(custom_function_t) * ccapacity);

    return (function_book_t){
            .plen = 0,
            .pcapacity = pcapacity,
            .pfns = pfns,
            .clen = 0,
            .ccapacity = ccapacity,
            .cfns = cfns,
    };
}

void function_book_free(function_book_t *functions) {
    for (unsigned int i = 0; i < functions->plen; i += 1) {
        primitive_function_free(&functions->pfns[i]);
    }

    free(functions->pfns);
    functions->pfns = NULL;
    functions->plen = 0;
    functions->pcapacity = 0;

    for (unsigned int i = 0; i < functions->clen; i += 1) {
        custom_function_free(&functions->cfns[i]);
    }

    free(functions->cfns);
    functions->cfns = NULL;
    functions->clen = 0;
    functions->ccapacity = 0;
}

bool function_book_push_primitive(function_book_t *functions, primitive_function_t pfn) {
    if (functions->plen >= functions->pcapacity) {
        functions->pcapacity *= 2;
        primitive_function_t *tmp_pfns = realloc(functions->pfns, sizeof(primitive_function_t) * functions->pcapacity);

        if (tmp_pfns) {
            functions->pfns = tmp_pfns;
        } else {
            return false;
        }
    }

    functions->pfns[functions->plen] = pfn;
    functions->plen += 1;

    return true;
}

bool function_book_contains_primitive(function_book_t *functions, const char *name, primitive_function_t *pfn) {
    for (unsigned int i = 0; i < functions->plen; i += 1) {
        if (strcmp(functions->pfns[i].name, name) == 0) {
            if (pfn) {
                *pfn = functions->pfns[i];
            }
            return true;
        }
    }

    return false;
}

bool function_book_push_custom(function_book_t *functions, custom_function_t cfn) {
    if (functions->clen >= functions->ccapacity) {
        functions->ccapacity *= 2;
        custom_function_t *tmp_cfns = realloc(functions->cfns, sizeof(custom_function_t) * functions->ccapacity);

        if (tmp_cfns) {
            functions->cfns = tmp_cfns;
        } else {
            return false;
        }
    }

    functions->cfns[functions->clen] = cfn;
    functions->clen += 1;

    return true;
}

bool function_book_contains_custom(function_book_t *functions, const char *name, custom_function_t *cfn) {
    for (unsigned int i = 0; i < functions->clen; i += 1) {
        if (strcmp(functions->cfns[i].name, name) == 0) {
            if (cfn) {
                *cfn = functions->cfns[i];
            }
            return true;
        }
    }

    return false;
}
