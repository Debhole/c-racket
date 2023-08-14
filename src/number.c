#include "number.h"

#include <ctype.h>
#include <stdlib.h>

rational_number_t rational_number_new(int numerator, int denominator) {
    return (rational_number_t) {
        .numerator = numerator,
        .denominator = denominator,
    };
}

real_number_t real_number_new(double value) {
    return (real_number_t) {
        .value = value,
    };
}

complex_number_t complex_number_new(double real, double complex) {
    return (complex_number_t) {
        .real = real,
        .complex = complex,
    };
}

bool try_to_rational_number(char *c, size_t size, rational_number_t *num) {
    return true;
}

bool try_to_real_number(char *c, size_t size, real_number_t *num) {
    return true;
}

bool try_to_complex_number(char *c, size_t size, complex_number_t *num) {
    return true;
}