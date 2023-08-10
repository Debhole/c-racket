#pragma once

#include "scanner.h"
#include "tokens/token.h"
#include "tokens/token_list.h"

#include "test_macros.h"

#include <string.h>

inline result_t test_scanner() {
    scanner_t s = scanner_new(
            ";Does this read basic delimiters correctly?\n"
            "([{}]) '`, ;Ignore the rest ()[\\a\n");

    token_list_t tokens = scanner_get_tokens(&s);

    // We expect 11 tokens
    // ( [ { } ] ) ' ` , "" EOF
    assert(tokens.len == 10);

    assert(tokens.tokens[0].type == TOKEN_LEFT_PAREN);
    assert(tokens.tokens[1].type == TOKEN_LEFT_BRACE);
    assert(tokens.tokens[2].type == TOKEN_LEFT_BRACKET);
    assert(tokens.tokens[3].type == TOKEN_RIGHT_BRACKET);
    assert(tokens.tokens[4].type == TOKEN_RIGHT_BRACE);
    assert(tokens.tokens[5].type == TOKEN_RIGHT_PAREN);
    assert(tokens.tokens[6].type == TOKEN_QUOTE);
    assert(tokens.tokens[7].type == TOKEN_QUASIQUOTE);
    assert(tokens.tokens[8].type == TOKEN_UNQUOTE);
    //assert(tokens.tokens[9].type == TOKEN_STRING);
    assert(tokens.tokens[9].type == TOKEN_EOF);

    return SUCCESS;
}