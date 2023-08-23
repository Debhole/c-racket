#include "test_macros.h"

#include "tokens/token_list.h"

result_t test_token_list(void) {
    token_list_t list = token_list_new();

    token_list_push(&list, token_boolean(true, 1));
    token_list_push(&list, token_new(TOKEN_LEFT_PAREN, 1));
    token_list_push(&list, token_new(TOKEN_RIGHT_PAREN, 1));
    assert(list.capacity == 8);
    assert(list.len == 3);

    token_t token;
    assert(token_list_get(&list, 0, &token) && token.type == TOKEN_BOOLEAN && *(bool *) token.data == true);
    assert(token_list_get(&list, 1, &token) && token.type == TOKEN_LEFT_PAREN && token.data == NULL);
    assert(token_list_get(&list, 2, &token) && token.type == TOKEN_RIGHT_PAREN);
    assert(!token_list_get(&list, 3, &token) && token.type == TOKEN_RIGHT_PAREN);

    token_list_free(&list);
    assert(list.capacity == 0);
    assert(list.len == 0);
    assert(!token_list_get(&list, 0, &token));

    list = token_list_new();
    for (int i = 0; i < 65; i += 1) {
        token_list_push(&list, token_real(real_number_new(5.0), 1));
    }
    assert(list.capacity == 128);
    assert(list.len == 65);

    token_list_free(&list);
    assert(list.capacity == 0);
    assert(list.len == 0);
    assert(list.tokens == NULL);

    return SUCCESS;
}
