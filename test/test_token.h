#pragma once

#include "test_macros.h"

#include "tokens/token.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

result_t test_token(void) {
    token_t token = token_string("Some Data", 1);
    assert(token.type == TOKEN_STRING);
    assert(token.line == 1);
    assert(strcmp(token.data, "Some Data") == 0);

    token_free(&token);
    assert(token.data == NULL);

    token = token_boolean(true, 1);
    assert(token.type == TOKEN_BOOLEAN);
    assert(token.line == 1);
    assert(*(bool *) token.data == true);

    token_free(&token);
    assert(token.data == NULL);

    return SUCCESS;
}
