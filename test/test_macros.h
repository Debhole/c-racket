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

#define assert_rationaleq(rnum, num, den)                         \
    assert((*(rational_number_t *) rnum).numerator == num); \
    assert((*(rational_number_t *) rnum).denominator == den)

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