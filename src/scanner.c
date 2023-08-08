#include "scanner.h"
#include "tokens/token_list.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

scanner_t scanner_new(const char *source) {
    size_t len = strlen(source);
    void *data = malloc(len);
    memcpy(data, source, len + 1);

    return (scanner_t){
            .source = data,
            .len = len,
            .loc = 0,
            .line = 1,
    };
}

void scanner_free(scanner_t *s) {
    s->len = 0;
    s->loc = 0;
    s->line = 0;
}

token_list_t scanner_get_tokens(scanner_t *s) {
    token_list_t list = token_list_new();

    while (scanner_consume_whitespace(s) && scanner_has_next(s)) {
        token_t token = scanner_next_token(s);

        token_list_push(&list, token);
    }
    token_t eof = token_new(TOKEN_EOF, s->line);
    token_list_push(&list, eof);

    return list;
}

token_t scanner_next_token(scanner_t *s) {
    char curr = scanner_advance(s);

    // Line comments
    while (curr == ';') {
        curr = scanner_advance_line(s);
    }

    switch (curr) {
        case '(': {
            return token_new(TOKEN_LEFT_PAREN, s->line);
        }
        case ')': {
            return token_new(TOKEN_RIGHT_PAREN, s->line);
        }

        // String Literal
        case '"': {
            char string[2048];
            int loc = 0;
            while ((curr = scanner_advance(s)) != '"') {
                string[loc] = curr;
                loc += 1;
            }
            string[loc] = '\0';
            return token_string(string, s->line);
        }

        case '\0': {
            return token_new(TOKEN_EOF, s->line);
        }
    }
}

char scanner_peek(scanner_t *s) {
    if (scanner_has_next(s)) {
        return s->source[s->loc];
    } else {
        return '\0';
    }
}

char scanner_advance(scanner_t *s) {
    char next = scanner_peek(s);
    s->loc += 1;

    return next;
}

char scanner_advance_line(scanner_t *s) {
    char next = scanner_advance(s);

    while (next != '\n') {
        next = scanner_advance(s);
    }

    s->line += 1;
    return scanner_advance(s);
}

bool scanner_consume_whitespace(scanner_t *s) {
    char next = scanner_peek(s);

    // Whitespace
    while (next == ' ' || next == '\r' || next == '\t' || next == '\n') {
        if (next == '\n') {
            s->line += 1;
        }

        scanner_advance(s);
        next = scanner_peek(s);
    }

    return true;
}

bool scanner_has_next(scanner_t *s) {
    return s->loc < s->len;
}