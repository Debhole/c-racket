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

    eval = interpreter_eval(&interpreter, "(number? #false)\n(symbol? sym)");
    assert(eval.len == 2);
    assert(eval.trees[0]->data);
    assert(*((bool *)(eval.trees[0]->data)) == false);

    assert(eval.trees[1]->data);
    assert(*((bool *)(eval.trees[1]->data)) == true);

    ast_list_free(&eval);

    eval = interpreter_eval(&interpreter, "(boolean? (number? 100.1))\n(number? (number? 5/10))");
    assert(eval.len == 2);
    assert(eval.trees[0]->data);
    assert(*((bool *)(eval.trees[0]->data)) == true);

    assert(eval.trees[1]->data);
    assert(*((bool *)(eval.trees[1]->data)) == false);

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
