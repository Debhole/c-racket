#include "scanner.h"
#include "tokens/token_list.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

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
    token_list_t tokens = token_list_new();

    token_t t;
    while (scanner_try_next_token(s, &t)) {
        token_list_push(&tokens, t);

        if (t.type == TOKEN_EOF) {
            break;
        }
    }

    return tokens;
}

bool scanner_try_next_token(scanner_t *s, token_t *t) {
    char curr;

    // Skip leading whitespace and comments
    do {
        curr = scanner_advance(s);

        if (curr == ';') {
            scanner_jump_endl(s);
            curr = scanner_advance(s);
        }
    } while (isspace(curr));

    switch (curr) {
        case ';':
            scanner_jump_endl(s);
            return scanner_try_next_token(s, t);
        case '(':
            *t = token_new(TOKEN_LEFT_PAREN, s->line);
            return true;
        case ')':
            *t = token_new(TOKEN_RIGHT_PAREN, s->line);
            return true;
        case '[':
            *t = token_new(TOKEN_LEFT_BRACE, s->line);
            return true;
        case ']':
            *t = token_new(TOKEN_RIGHT_BRACE, s->line);
            return true;
        case '{':
            *t = token_new(TOKEN_LEFT_BRACKET, s->line);
            return true;
        case '}':
            *t = token_new(TOKEN_RIGHT_BRACKET, s->line);
            return true;
        case ',':
            *t = token_new(TOKEN_UNQUOTE, s->line);
            return true;
        case '`':
            *t = token_new(TOKEN_QUASIQUOTE, s->line);
            return true;
        case '\'':
            *t = token_new(TOKEN_QUOTE, s->line);
            return true;
        case '\0':
            *t = token_new(TOKEN_EOF, s->line);
            return true;
        default:
            return false;
    }
}

bool scanner_at_end(scanner_t *s) {
    return s->loc >= strlen(s->source);
}

char scanner_peek(scanner_t *s) {
    if (scanner_at_end(s)) {
        return '\0';
    } else {
        char next = s->source[s->loc];
        return next;
    }
}

char scanner_advance(scanner_t *s) {
    char curr = scanner_peek(s);
    s->loc += 1;

    return curr;
}

void scanner_jump_endl(scanner_t *s) {
    char curr;

    do {
        curr = scanner_advance(s);
    } while (curr != '\0' && curr != '\n');
}

bool is_delimiter(char c) {
    const char delimiters[] = "()[]{}\",'`;";

    return (strchr(delimiters, c) != NULL) || isspace(c);
}