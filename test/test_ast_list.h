#pragma once

#include "test_macros.h"

#include "scanner/scanner.h"

result_t test_ast_list() {
    scanner_t s = scanner_new("(+ 4 5) 6 \"Hello, World!\" (define Hello \"World!\")");
    token_list_t tokens = scanner_get_tokens(&s);
    ast_builder_t b = ast_builder_new(&tokens);

    ast_list_t trees = ast_builder_get_trees(&b);

    assert(trees.len == 4);

    ast_node_t *tree;

    // (+ 4 5)
    assert(ast_list_get(&trees, 0, &tree));
    assert(tree->tag == TAG_EXPRESSION);
    assert(tree->num_children == 2);
    assert_streq((char *)tree->data, "+");

    assert(tree->children[0]->tag == TAG_RATIONAL);
    assert_rationaleq(tree->children[0]->data, 4, 1);
    assert(tree->children[1]->tag == TAG_RATIONAL);
    assert_rationaleq(tree->children[1]->data, 5, 1);

    // 6
    assert(ast_list_get(&trees, 1, &tree));
    assert(tree->tag == TAG_RATIONAL);
    assert(tree->num_children == 0);
    assert_rationaleq(tree->data, 6, 1);

    // "Hello, World!"
    assert(ast_list_get(&trees, 2, &tree));
    assert(tree->tag == TAG_STRING);
    assert(tree->num_children == 0);
    assert_streq((char *)tree->data, "Hello, World!");

    // (define Hello "World!")
    assert(ast_list_get(&trees, 3, &tree));
    assert(tree->tag == TAG_EXPRESSION);
    assert(tree->num_children == 2);
    assert_streq((char *)tree->data, "define");

    assert(tree->children[0]->tag == TAG_SYMBOL);
    assert_streq((char *)tree->children[0]->data, "Hello");
    assert(tree->children[1]->tag == TAG_STRING);
    assert_streq((char *)tree->children[1]->data, "World!");

    scanner_free(&s);
    ast_builder_free(&b);
    ast_list_free(&trees);

    return SUCCESS;
}