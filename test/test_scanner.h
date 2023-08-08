#pragma once

#include "tokens/token_list.h"
#include "tokens/token.h"
#include "scanner.h"

#include "test_macros.h"

#include <string.h>

inline result_t test_scanner() {
    scanner_t s = scanner_new("  (  ) ((   ");
    token_list_t list = scanner_get_tokens(&s);
    assert(list.len == 5);

    token_t token;
    assert(token_list_get(&list, 0, &token) && token.type == TOKEN_LEFT_PAREN);
    assert(token_list_get(&list, 1, &token) && token.type == TOKEN_RIGHT_PAREN);
    assert(token_list_get(&list, 2, &token) && token.type == TOKEN_LEFT_PAREN);
    assert(token_list_get(&list, 3, &token) && token.type == TOKEN_LEFT_PAREN);
    assert(token_list_get(&list, 4, &token) && token.type == TOKEN_EOF);

    scanner_free(&s);
    token_list_free(&list);

    s = scanner_new("    \n\r  \t  ");
    list = scanner_get_tokens(&s);
    assert(list.len == 1);

    scanner_free(&s);
    token_list_free(&list);

    s = scanner_new("(\"String\")");
    list = scanner_get_tokens(&s);
    assert(list.len == 4);

    return SUCCESS;
}