#pragma once

#include "token.h"

#include <stdbool.h>

typedef struct token_list_t {
    int len;
    int capacity;
    token_t *tokens;
} token_list_t;

token_list_t token_list_new();
void token_list_free(token_list_t *list);

bool token_list_push(token_list_t *list, token_t token);
bool token_list_get(token_list_t *list, int idx, token_t *token);