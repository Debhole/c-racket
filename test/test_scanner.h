#pragma once

#include "tokens/token_list.h"
#include "tokens/token.h"
#include "scanner.h"

#include "test_macros.h"

#include <string.h>

inline result_t test_scanner() {
    token_list_t list = scan_text("(+ 1 2 3 4 5)", 13);
    assert(list.len == 8);

    token_t token;
    assert(token_list_get(&list, 0, &token) && token.type == TOKEN_LEFT_PAREN);
    assert(token_list_get(&list, 1, &token) && token.type == TOKEN_SYMBOL && strcmp(token.data, "+") == 0);
    assert(token_list_get(&list, 7, &token) && token.type == TOKEN_RIGHT_PAREN);

    return SUCCESS;
}