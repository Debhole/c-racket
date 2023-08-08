#include "token.h"

#include "token_type.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

token_t token_new(token_type_t type, int line) {
    return (token_t){
            .type = type,
            .data = NULL,
            .line = line};
}

void token_free(token_t *token) {
    if (token == NULL) {
        return;
    }

    if (token->data != NULL) {
        free(token->data);
    }
    token->data = NULL;
    token->line = 0;
}

token_t token_string(const char *value, int line) {
    size_t size = strlen(value) + 1;
    void *data = malloc(size);
    memcpy(data, value, size);

    return (token_t){
        .type = TOKEN_STRING,
        .data = data,
        .line = line,
    };
}

token_t token_number(double value, int line) {
    void *data = malloc(sizeof(double));
    memcpy(data, &value, sizeof(double));

    return (token_t){
        .type = TOKEN_NUMBER,
        .data = data,
        .line = line,
    };
}
token_t token_boolean(bool value, int line) {
    void *data = malloc(sizeof(bool));
    memcpy(data, &value, sizeof(bool));

    return (token_t){
        .type = TOKEN_BOOLEAN,
        .data = data,
        .line = line,
    };
}

token_t token_symbol(const char *value, int line) {
    size_t size = strlen(value) + 1;
    void *data = malloc(size);
    memcpy(data, value, size);

    return (token_t){
            .type = TOKEN_SYMBOL,
            .data = data,
            .line = line,
    };
}

token_t token_error(const char *value, int line) {
    size_t size = strlen(value) + 1;
    void *data = malloc(size);
    memcpy(data, value, size);

    return (token_t){
        .type = TOKEN_ERROR,
        .data = data,
        .line = line,
    };
}

void token_print(const token_t *t) {
    const char *token_type = token_type_to_string(t->type);

    switch (t->type) {
        case TOKEN_STRING:
            printf("[%s -\"%s\" - Line %d ]\n", token_type, (const char *)t->data, t->line);
            break;
        case TOKEN_NUMBER:
            printf("[%s - %f - Line %d]\n", token_type, *(double *)t->data, t->line);
            break;
        case TOKEN_BOOLEAN: {
            const char *value = *(bool *)t->data ? "#true" : "#false";
            printf("[%s - %s - Line %d]\n", token_type, value, t->line);
            break;
        }
        case TOKEN_SYMBOL:
            printf("[%s - '%s - Line %d]\n", token_type, (const char *)t->data, t->line);
            break;
        default:
            printf("[%s - Line %d]\n", token_type, t->line);
            break;
    }
}