#include "token_list.h"

#include <stdbool.h>
#include <stdlib.h>

token_list_t token_list_new() {
    int capacity = 8;
    token_t *tokens = malloc(sizeof(token_t) * capacity);

    return (token_list_t){
            .len = 0,
            .capacity = capacity,
            .tokens = tokens,
    };
}

void token_list_free(token_list_t *list) {
    for (int i = 0; i < list->len; i += 1) {
        token_free(&list->tokens[i]);
    }

    free(list->tokens);
    list->tokens = NULL;
    list->len = 0;
    list->capacity = 0;
}

bool token_list_push(token_list_t *list, token_t token) {
    if (list->len >= list->capacity) {
        list->capacity *= 2;
        token_t *new_list = realloc(list->tokens, sizeof(token_t) * list->capacity);

        if (new_list) {
            list->tokens = new_list;
        } else {
            return false;
        }
    }

    list->tokens[list->len] = token;
    list->len += 1;

    return true;
}

bool token_list_get(token_list_t *list, unsigned int idx, token_t *token) {
    if (idx >= list->len) {
        return false;
    } else {
        (*token) = list->tokens[idx];
        return true;
    }
}