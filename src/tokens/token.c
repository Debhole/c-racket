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

token_t token_integer(int num, unsigned int line) {
    size_t size = sizeof(int);

    void *data = malloc(size);
    memcpy(data, &num, size);

    return (token_t){
            .type = TOKEN_INTEGER,
            .data = data,
            .data_size = size,
            .line = line,
    };
}

token_t token_double(double num, unsigned int line) {
    size_t size = sizeof(double);
    void *data = malloc(size);
    memcpy(data, &num, size);

    return (token_t){
            .type = TOKEN_DOUBLE,
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

void print_string(const token_t *t) {
    printf("[%s - \"%s\" - Line %d]\n", token_type_to_string(t->type), (const char *)t->data, t->line);
}

void print_integer(const token_t *t) {
    printf("[%s - %d - Line %d]\n", token_type_to_string(t->type), *(int *)t->data, t->line);
}

void print_double(const token_t *t) {
    printf("[%s - %f - Line %d]\n", token_type_to_string(t->type), *(double *)t->data, t->line);
}

void print_boolean(const token_t *t) {
    const char *value = *(bool *)t->data ? "#true" : "#false";
    printf("[%s - %s - Line %d]\n", token_type_to_string(t->type), value, t->line);
}

// Add other handlers similarly...

typedef void (*print_handler_t)(const token_t *);
print_handler_t handlers[] = {
    [TOKEN_STRING] = print_string,
    [TOKEN_INTEGER] = print_integer,
    [TOKEN_DOUBLE] = print_double,
    [TOKEN_BOOLEAN] = print_boolean,
    // Add more mappings as needed...
};

void token_print(const token_t *t) {
    if (handlers[t->type]) {
        handlers[t->type](t);
    } else {
        printf("[%s - Line %d]\n", token_type_to_string(t->type), t->line);
    }
}



}
