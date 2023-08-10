#pragma once

#include "scanner.h"
#include "tokens/token.h"
#include "tokens/token_list.h"

#include "test_macros.h"

#include <string.h>

inline result_t test_scanner() {
    scanner_t s = scanner_new(
            ";Does this read basic delimiters correctly?\n"
            "([{}]) '`, ;Ignore the rest ()[\\a\n"
            "\"This is a string\"");

    token_list_t tokens = scanner_get_tokens(&s);

    // We expect 11 tokens
    // ( [ { } ] ) ' ` , "" EOF
    assert(tokens.len == 11);

    assert(tokens.tokens[0].type == TOKEN_LEFT_PAREN);
    assert(tokens.tokens[1].type == TOKEN_LEFT_BRACE);
    assert(tokens.tokens[2].type == TOKEN_LEFT_BRACKET);
    assert(tokens.tokens[3].type == TOKEN_RIGHT_BRACKET);
    assert(tokens.tokens[4].type == TOKEN_RIGHT_BRACE);
    assert(tokens.tokens[5].type == TOKEN_RIGHT_PAREN);
    assert(tokens.tokens[6].type == TOKEN_QUOTE);
    assert(tokens.tokens[7].type == TOKEN_QUASIQUOTE);
    assert(tokens.tokens[8].type == TOKEN_UNQUOTE);
    assert(tokens.tokens[9].type == TOKEN_STRING);
    assert(tokens.tokens[10].type == TOKEN_EOF);

    scanner_free(&s);
    token_list_free(&tokens);

    return SUCCESS;
}

inline result_t test_scanner_strings() {
    scanner_t s = scanner_new(
            "\"Hello \\\n"
            "Newline!\"");
    token_list_t tokens = scanner_get_tokens(&s);

    assert(tokens.len == 2);

    assert(tokens.tokens[0].type == TOKEN_STRING);
    assert(strcmp(tokens.tokens[0].data, "Hello Newline!") == 0);

    scanner_free(&s);
    token_list_free(&tokens);

    s = scanner_new(
            "\"Hi\\40There!\""
            "\"Does \\\"this\\\" show up \\\\correctly\\\\?\""
            "\"How about these? \\\'\\a\\b\\t\\n\\v\\f\\r\\\'\""
            );
    tokens = scanner_get_tokens(&s);

    assert(tokens.len == 4);

    assert(tokens.tokens[0].type == TOKEN_STRING);
    assert(strcmp(tokens.tokens[0].data, "Hi There!") == 0);
    assert(strcmp(tokens.tokens[1].data, "Does \"this\" show up \\correctly\\?") == 0);
    assert(strcmp(tokens.tokens[2].data, "How about these? \'\a\b\t\n\v\f\r\'") == 0);

    return SUCCESS;
}