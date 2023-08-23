#pragma once

#include "test_macros.h"

#include "ast/ast.h"
#include "ast/ast_builder.h"
#include "scanner/scanner.h"
#include "tokens/token_list.h"

result_t test_ast_leaves(void) {
    // #true
    token_list_t tokens = token_list_new();
    token_list_push(&tokens, token_boolean(true, 1));
    token_list_push(&tokens, token_new(TOKEN_EOF, 1));

    ast_builder_t b = ast_builder_new(&tokens);
    ast_node_t *tree = ast_builder_next_tree(&b);

    assert(tree->num_children == 0);
    assert(*(bool *)tree->data == true);
    assert(tree->tag == TAG_BOOLEAN);

    ast_node_free(tree);
    ast_builder_free(&b);

    // (+ 4 3)
    tokens = token_list_new();
    token_list_push(&tokens, token_new(TOKEN_LEFT_PAREN, 1));
    token_list_push(&tokens, token_symbol("+", 1));
    token_list_push(&tokens, token_rational(rational_number_new(4, 1), 1));
    token_list_push(&tokens, token_rational(rational_number_new(3, 1), 1));
    token_list_push(&tokens, token_new(TOKEN_RIGHT_PAREN, 1));
    token_list_push(&tokens, token_new(TOKEN_EOF, 1));

    b = ast_builder_new(&tokens);
    tree = ast_builder_next_tree(&b);

    assert(tree->tag == TAG_EXPRESSION);
    assert_streq("+", (char *)tree->data);
    assert(tree->num_children == 2);

    ast_node_t *left = tree->children[0];
    assert(left->tag == TAG_RATIONAL);
    assert_rationaleq(left->data, 4, 1);
    assert(left->num_children == 0);

    ast_node_t *right = tree->children[1];
    assert(right->tag == TAG_RATIONAL);
    assert_rationaleq(right->data, 3, 1);
    assert(right->num_children == 0);

    ast_node_free(tree);
    ast_builder_free(&b);

    return SUCCESS;
}

result_t test_ast_nested(void) {
    scanner_t s = scanner_new("(+ 3 4 (- 4 1))");
    token_list_t tokens = scanner_get_tokens(&s);
    ast_builder_t b = ast_builder_new(&tokens);

    ast_node_t *tree = ast_builder_next_tree(&b);

    assert(tree->tag == TAG_EXPRESSION);
    assert(tree->num_children == 3);
    assert_streq((char *)tree->data, "+");

    // 3
    ast_node_t *left = tree->children[0];
    assert(left->tag == TAG_RATIONAL);
    assert(left->num_children == 0);
    assert_rationaleq(left->data, 3, 1);

    // 4
    ast_node_t *mid = tree->children[1];
    assert(mid->tag == TAG_RATIONAL);
    assert(mid->num_children == 0);
    assert_rationaleq(mid->data, 4, 1);

    // (- 4 1)
    ast_node_t *right = tree->children[2];
    assert(right->tag == TAG_EXPRESSION);
    assert(right->num_children == 2);
    assert_streq((char *)right->data, "-");

    // 4
    ast_node_t *nested_left = right->children[0];
    assert(nested_left->tag == TAG_RATIONAL);
    assert(nested_left->num_children == 0);
    assert_rationaleq(nested_left->data, 4, 1);

    // 1
    ast_node_t *nested_right = right->children[1];
    assert(nested_right->tag == TAG_RATIONAL);
    assert(nested_right->num_children == 0);
    assert_rationaleq(nested_right->data, 1, 1);

    ast_node_free(tree);
    ast_builder_free(&b);
    scanner_free(&s);

    return SUCCESS;
}
