#pragma once

#include "ast/ast_list.h"
#include "interpreter/interpreter.h"
#include "test_macros.h"

result_t test_basic_functions(void) {
    interpreter_t interpreter = interpreter_new();

    ast_list_t eval = interpreter_eval(&interpreter, "(boolean? #true)");
    assert(eval.len == 1);
    assert(eval.trees[0]->data);
    assert(*((bool *)(eval.trees[0]->data)) == true);

    ast_list_free(&eval);

    eval = interpreter_eval(&interpreter, "(number? #false)\n");
    assert(eval.len == 1);
    assert(eval.trees[0]->data);
    assert(*((bool *)(eval.trees[0]->data)) == false);

    ast_list_free(&eval);

    eval = interpreter_eval(&interpreter, "(boolean? (number? 100.1))\n(number? (number? 0.5))");
    assert(eval.len == 2);
    assert(eval.trees[0]->data);
    assert(*((bool *)(eval.trees[0]->data)) == true);

    assert(eval.trees[1]->data);
    assert(*((bool *)(eval.trees[1]->data)) == false);

    ast_list_free(&eval);

    eval = interpreter_eval(&interpreter, "(number? -5.5)");
    assert(*((bool *)(eval.trees[0]->data)) == true);

    ast_list_free(&eval);
    interpreter_free(&interpreter);

    return SUCCESS;
}

result_t test_complex_functions(void) {
    interpreter_t interpreter = interpreter_new();

    ast_list_t eval = interpreter_eval(&interpreter, "(+ 1 2 3 4 5) (- 5 4 3 2 1) (* 1 2 3 4 5)");
    assert(eval.len == 3);
    assert_inteq(eval.trees[0]->data, 15);
    assert_inteq(eval.trees[1]->data, -5);
    assert_inteq(eval.trees[2]->data, 120);

    ast_list_free(&eval);
    interpreter_free(&interpreter);

    return SUCCESS;
}

result_t test_string_functions(void) {
    interpreter_t interpreter = interpreter_new();

    ast_list_t eval = interpreter_eval(&interpreter, "(string-append) (string-append \"Hello\" \", \" \"World!\")");
    assert(eval.len == 2);
    assert_streq(eval.trees[0]->data, "");
    assert_streq(eval.trees[1]->data, "Hello, World!");

    ast_list_free(&eval);
    interpreter_free(&interpreter);

    return SUCCESS;
}

result_t test_define_value(void) {
    interpreter_t interpreter = interpreter_new();

    ast_list_t eval = interpreter_eval(&interpreter, "(define HELLO \"Hello\") (define WORLD \"World!\"");
    assert(eval.len == 0);

    ast_list_free(&eval);

    eval = interpreter_eval(&interpreter, "HELLO WORLD (string-append HELLO WORLD)");
    assert(eval.len == 3);
    assert_streq(eval.trees[0]->data, "Hello");
    assert_streq(eval.trees[1]->data, "World!");
    assert_streq(eval.trees[2]->data, "HelloWorld!");

    ast_list_free(&eval);
    interpreter_free(&interpreter);

    return SUCCESS;
}

result_t test_define_fn(void) {
    interpreter_t interpreter = interpreter_new();

    ast_list_t eval = interpreter_eval(&interpreter, "(define (add1 x) (+ x 1))");
    assert(eval.len == 0);

    assert(interpreter.function_book.clen == 1);
    assert_streq(interpreter.function_book.cfns[0].name, "add1");

    ast_list_free(&eval);

    eval = interpreter_eval(&interpreter, "(add1 6) (add1 7)");
    assert(eval.len == 2);
    assert_inteq(eval.trees[0]->data, 7);
    assert_inteq(eval.trees[1]->data, 8);

    ast_list_free(&eval);
    interpreter_free(&interpreter);

    return SUCCESS;
}

result_t test_if_function(void) {
    interpreter_t interpreter = interpreter_new();

    ast_list_t eval = interpreter_eval(&interpreter, "(if #true 6 5) (if #false 6 5)");
    assert(eval.len == 2);
    assert_inteq(eval.trees[0]->data, 6);
    assert_inteq(eval.trees[1]->data, 5);

    ast_list_free(&eval);

    eval = interpreter_eval(&interpreter, "(if (number? #true) (string-append \"TR\" \"UE\") (+ 6 5))\n"
                                          "(if (number? -5.5) (string-append \"TR\" \"UE\") (+ 6 5))");
    assert(eval.len == 2);
    assert_inteq(eval.trees[0]->data, 11);
    assert_streq(eval.trees[1]->data, "TRUE");

    ast_list_free(&eval);
    interpreter_free(&interpreter);

    return SUCCESS;
}

result_t test_and_function(void) {
    interpreter_t interpreter = interpreter_new();

    ast_list_t eval = interpreter_eval(&interpreter, "(and #true #true (number? 6))\n"
                                                     "(and #true (string? \"string\") (number? #true))");
    assert(eval.len == 2);
    assert(*((bool *)(eval.trees[0]->data)) == true);
    assert(*((bool *)(eval.trees[1]->data)) == false);

    ast_list_free(&eval);
    interpreter_free(&interpreter);

    return SUCCESS;
}

result_t test_or_function(void) {
    interpreter_t interpreter = interpreter_new();

    ast_list_t eval = interpreter_eval(&interpreter, "(or #false #false (number? #false) (string? \"true!\"))\n"
                                                     "(or #false #false (string? 5))");
    assert(eval.len == 2);
    assert(*((bool *)(eval.trees[0]->data)) == true);
    assert(*((bool *)(eval.trees[1]->data)) == false);

    ast_list_free(&eval);
    interpreter_free(&interpreter);

    return SUCCESS;
}

result_t test_numeq_function(void) {
    interpreter_t interpreter = interpreter_new();

    ast_list_t eval = interpreter_eval(&interpreter, "(= 5 (+ 3 2) (+ 1 2 2) 5.0) (= 5 5.0 5.000001)");
    assert(eval.len == 2);
    assert(*((bool *)(eval.trees[0]->data)) == true);
    assert(*((bool *)(eval.trees[1]->data)) == false);

    ast_list_free(&eval);
    interpreter_free(&interpreter);

    return SUCCESS;
}
