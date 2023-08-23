#include "token.h"

#include "keyword.h"
#include "number.h"
#include "token_type.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

token_t token_new(token_type_t type, unsigned int line) {
    return (token_t){
            .type = type,
            .data = NULL,
            .data_size = 0,
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
    token->data_size = 0;
    token->line = 0;
}

token_t token_string(const char *value, unsigned int line) {
    size_t size = strlen(value) + 1;
    void *data = malloc(size);
    memcpy(data, value, size);

    return (token_t){
            .type = TOKEN_STRING,
            .data = data,
            .data_size = size,
            .line = line,
    };
}

token_t token_boolean(bool value, unsigned int line) {
    size_t size = sizeof(bool);

    void *data = malloc(size);
    memcpy(data, &value, size);

    return (token_t){
            .type = TOKEN_BOOLEAN,
            .data = data,
            .data_size = size,
            .line = line,
    };
}

token_t token_rational(rational_number_t num, unsigned int line) {
    size_t size = sizeof(rational_number_t);

    void *data = malloc(size);
    memcpy(data, &num, size);

    return (token_t){
            .type = TOKEN_RATIONAL,
            .data = data,
            .data_size = size,
            .line = line,
    };
}

token_t token_real(real_number_t num, unsigned int line) {
    size_t size = sizeof(real_number_t);
    void *data = malloc(size);
    memcpy(data, &num, size);

    return (token_t){
            .type = TOKEN_REAL,
            .data = data,
            .data_size = size,
            .line = line,
    };
}

token_t token_symbol(const char *name, unsigned int line) {
    size_t size = strlen(name) + 1;
    void *data = malloc(size);
    memcpy(data, name, size);

    return (token_t){
            .type = TOKEN_SYMBOL,
            .data = data,
            .data_size = size,
            .line = line,
    };
}

token_t token_keyword(const char *name, unsigned int line) {
    size_t size = strlen(name) + 1;
    void *data = malloc(size);
    memcpy(data, name, size);

    return (token_t){
            .type = TOKEN_KEYWORD,
            .data = data,
            .data_size = size,
            .line = line,
    };
}

token_t token_error(const char *message, unsigned int line) {
    size_t size = strlen(message) + 1;
    void *data = malloc(size);
    memcpy(data, message, size);

    return (token_t){
            .type = TOKEN_ERROR,
            .data = data,
            .data_size = size,
            .line = line,
    };
}

void token_print(const token_t *t) {
    const char *token_type = token_type_to_string(t->type);

    switch (t->type) {
        case TOKEN_STRING:
            printf("[%s -\"%s\" - Line %d ]\n", token_type, (const char *) t->data, t->line);
            break;
        case TOKEN_RATIONAL: {
            rational_number_t *num = (rational_number_t *) t->data;
            if (num->denominator == 1) {
                printf("[%s - %d - Line %d]\n", token_type, num->numerator, t->line);
            } else {
                printf("[%s - %d/%d - Line %d]\n", token_type, num->numerator, num->denominator, t->line);
            }
            break;
        }
        case TOKEN_REAL: {
            real_number_t *num = (real_number_t *) t->data;

            printf("[%s - %f - Line %d]", token_type, num->value, t->line);
            break;
        }
        case TOKEN_BOOLEAN: {
            const char *value = *(bool *) t->data ? "#true" : "#false";
            printf("[%s - %s - Line %d]\n", token_type, value, t->line);
            break;
        }
        case TOKEN_SYMBOL:
        case TOKEN_KEYWORD:
            printf("[%s - '%s - Line %d]\n", token_type, (const char *) t->data, t->line);
            break;
        case TOKEN_ERROR:
            printf("[%s - '%s - Line %d]\n", token_type, (const char *) t->data, t->line);
            break;
        default:
            printf("[%s - Line %d]\n", token_type, t->line);
            break;
    }
}
