#include "token.h"

#include "token_type.h"

#include <stdlib.h>
#include <string.h>

token_t token_new(token_type_t type, int line) {
    return (token_t){
            .type = type,
            .data = NULL,
            .line = line};
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