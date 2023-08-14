#pragma once

#include "scanner/scanner.h"
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
    assert_streq(tokens.tokens[0].data, "Hello Newline!");

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
    assert_streq(tokens.tokens[0].data, "Hi There!");
    assert_streq(tokens.tokens[1].data, "Does \"this\" show up \\correctly\\?");
    assert_streq(tokens.tokens[2].data, "How about these? \'\a\b\t\n\v\f\r\'");

    scanner_free(&s);
    token_list_free(&tokens);

    return SUCCESS;
}

inline result_t test_scanner_bools() {
    scanner_t s = scanner_new("#true(#t)#T #f'#false`#F");
    token_list_t tokens = scanner_get_tokens(&s);

    assert(tokens.len == 11);

    assert(tokens.tokens[0].type == TOKEN_BOOLEAN);
    assert(* (bool *)tokens.tokens[0].data == true);
    assert(* (bool *)tokens.tokens[2].data == true);
    assert(* (bool *)tokens.tokens[4].data == true);

    assert(tokens.tokens[5].type == TOKEN_BOOLEAN);
    assert(* (bool *)tokens.tokens[5].data == false);
    assert(* (bool *)tokens.tokens[7].data == false);
    assert(* (bool *)tokens.tokens[9].data == false);

    assert(tokens.tokens[1].type == TOKEN_LEFT_PAREN);
    assert(tokens.tokens[3].type == TOKEN_RIGHT_PAREN);
    assert(tokens.tokens[6].type == TOKEN_QUOTE);
    assert(tokens.tokens[8].type == TOKEN_QUASIQUOTE);

    scanner_free(&s);
    token_list_free(&tokens);

    return SUCCESS;
}

inline result_t test_scanner_datum() {
    scanner_t s = scanner_new("this_is_a_datum#12345 (other junk...)");
    char buf[2048];

    assert(scanner_try_next_datum(&s, buf, sizeof buf));
    assert_streq(buf, "this_is_a_datum#12345");

    return SUCCESS;
}