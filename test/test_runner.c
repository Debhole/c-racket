#include "test_macros.h"
#include "test_scanner.h"
#include "test_token.h"
#include "test_token_list.h"

#include <stdio.h>

int main() {
    printf("*****************\n* Running Tests *\n*****************\n");

    init_tests();

    record_test(test_token_list);
    record_test(test_scanner);
    record_test(test_token);

    report_tests();
}