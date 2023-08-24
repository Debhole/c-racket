#include "test_macros.h"

#include "test_ast.h"
#include "test_ast_list.h"
#include "test_scanner.h"
#include "test_token.h"
#include "test_token_list.h"
#include "test_interpreter.h"

#include <stdio.h>

int main(void) {
    printf("*****************\n* Running Tests *\n*****************\n");

    init_tests();

    record_test(test_token);

    record_test(test_token_list);

    record_test(test_scanner);
    record_test(test_scanner_strings);
    record_test(test_scanner_bools);
    record_test(test_scanner_datum);
    record_test(test_scanner_numbers);
    record_test(test_scanner_symbols);
    record_test(test_scanner_all);

    record_test(test_ast_leaves);
    record_test(test_ast_nested);

    record_test(test_ast_list);

    record_test(test_ast_mem);

    record_test(test_basic_functions);
    record_test(test_complex_functions);

    report_tests();
}
