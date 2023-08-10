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
        // Line comments
        case ';':
            scanner_jump_endl(s);
            return scanner_try_next_token(s, t);

        // Delimiters
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

        // String Literals
        case '"':
            return scanner_try_next_string(s, t);

        default:
            return false;
    }
}

bool scanner_try_next_string(scanner_t *s, token_t *t) {
    unsigned int start_line = s->line;
    char curr;

    char buf[2048];
    unsigned int loc = 0;

    // Read until next double quote (")

    while (scanner_str_advance(s, &curr)) {
        if (curr == '\0' || loc >= 2047) {
            // No closing double quotes was found, or string was too long
            return false;
        } else {
            buf[loc] = curr;
        }
        loc += 1;
    }

    buf[loc] = '\0';
    *t = token_string(buf, start_line);
    return true;
}

bool scanner_at_end(scanner_t *s) {
    return s->loc >= s->len;
}

char scanner_peek(scanner_t *s) {
    if (scanner_at_end(s)) {
        return '\0';
    } else {
        char next = s->source[s->loc];
        return next;
    }
}

char scanner_peek_two(scanner_t *s) {
    if (s->loc + 1 >= s->len) {
        return '\0';
    } else {
        char in_two = s->source[s->loc + 1];
        return in_two;
    }
}

char scanner_advance(scanner_t *s) {
    char curr = scanner_peek(s);
    s->loc += 1;

    return curr;
}

bool scanner_str_advance(scanner_t *s, char *c) {
    char curr = scanner_advance(s);

    if (curr == '"') {
        return false;
    } else if (curr == '\\') {
        curr = scanner_advance(s);

        // Octal number encoding
        // This feels like a bad way to do this... Maybe come up with a better
        // solution later?
        if (isdigit(curr) && curr <= '7') {
            char num[4];
            num[0] = curr;

            if (isdigit(scanner_peek_two(s)) && isdigit(scanner_peek(s))) {
                curr = scanner_advance(s);
                num[1] = curr;

                curr = scanner_advance(s);
                num[2] = curr;

                num[3] = '\0';
            } else if (isdigit(scanner_peek(s))) {
                curr = scanner_advance(s);
                num[1] = curr;

                num[2] = '\0';
            } else {
                num[1] = '\0';
            }

            int value = strtol(num, NULL, 8);

            if (value > 255) {
                return false;
            } else {
                *c = (char)value;
                return true;
            }
        }

        switch (curr) {
            case 'a':
                *c = '\a';
                return true;
            case 'b':
                *c = '\b';
                return true;
            case 't':
                *c = '\t';
                return true;
            case 'n':
                *c = '\n';
                return true;
            case 'v':
                *c = '\v';
                return true;
            case 'f':
                *c = '\f';
                return true;
            case 'r':
                *c = '\r';
                return true;
            case 'e':
                // ESC has no escape sequence in C... but the ASCII
                // character code is 27
                *c = 27;
                return true;
            case '"':
                *c = '"';
                return true;
            case '\'':
                *c = '\'';
                return true;
            case '\\':
                *c = '\\';
                return true;
            case '\n':
                return scanner_str_advance(s, c);
            case '\r':
                if (scanner_peek(s) == '\n') {
                    s->loc += 1;
                }
                return scanner_str_advance(s, c);

            // Hexadecimal number encoding
            case 'x':
            case 'u':
            case 'U':
            default:
                return false;
        }
    } else {
        *c = curr;
        return true;
    }
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