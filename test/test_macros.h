#pragma once

#include <stdio.h>

typedef enum result_t {
    SUCCESS,
    FAILURE
} result_t;

#define assert(x)                 \
    do {                          \
        if (!(x)) return FAILURE; \
    } while (0)

#define assert_streq(tokstr, str) assert(strcmp(tokstr, str) == 0)

#define assert_symeq(tok, str)        \
    assert(tok.type == TOKEN_SYMBOL); \
    assert_streq((char *) tok.data, str)

#define assert_kweq(tok, str)          \
    assert(tok.type == TOKEN_KEYWORD); \
    assert_streq((char *) tok.data, str)

#define assert_inteq(val, num) assert(*(int *)val == num)

#define assert_double_approxeq(val, num, epsilon) \
    assert(((*(double *)val - num) < epsilon) && ((num - *(double *)val) < epsilon))

#define init_tests() int num_fails = 0

#define report_tests() return num_fails

#define record_test(x)                   \
    do {                                 \
        if (x() == SUCCESS) {            \
            printf(#x " --- Success\n"); \
        } else {                         \
            num_fails += 1;              \
            printf(#x " --- Failure\n"); \
        }                                \
    } while (0)
