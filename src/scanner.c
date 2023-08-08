#include "scanner.h"
#include "tokens/token_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
        // Expression Bounds
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
            int line = s->line;
            while ((curr = scanner_advance(s)) != '"') {
                string[loc] = curr;
                loc += 1;

                if (curr == '\0' || loc == 2048) {
                    return token_error("String was not terminated or exceeds the 2047 character limit.", line);
                }
            }
            string[loc] = '\0';
            return token_string(string, s->line);
        }

        // Number Literal
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            return scanner_next_number(s);
        }

        // Possibly Number Literal or Symbol
        case '.': {
            if (isdigit(scanner_peek(s))) {
                return scanner_next_number(s);
            }
        }
        case '-':
        case '+': {
            char peek = scanner_peek(s);
            if (isdigit(peek) || (peek == '.' && isdigit(scanner_peek_two(s)))) {
                return scanner_next_number(s);
            }
            // Handle symbols later...
            return token_error("Unhandled Case", s->line);
        }

        // EOF
        case '\0': {
            return token_new(TOKEN_EOF, s->line);
        }

        default: {
            return token_error("Unknown Character", s->line);
        }
    }
}

token_t scanner_next_number(scanner_t *s) {
    char *end;
    double number = strtod(s->source + s->loc - 1, &end);

    s->loc = end - s->source;

    return token_number(number, s->line);
}

char scanner_peek(scanner_t *s) {
    if (scanner_has_next(s)) {
        return s->source[s->loc];
    } else {
        return '\0';
    }
}

char scanner_peek_two(scanner_t *s) {
    if ((s->loc + 1) < s->len) {
        return s->source[s->loc + 1];
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