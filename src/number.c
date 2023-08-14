#include "number.h"

#include <ctype.h>
#include <stdlib.h>

rational_number_t rational_number_new(int numerator, int denominator) {
    return (rational_number_t){
            .numerator = numerator,
            .denominator = denominator,
    };
}

real_number_t real_number_new(double value) {
    return (real_number_t){
            .value = value,
    };
}

bool try_to_rational_number(char *c, size_t size, int base, rational_number_t *num) {
    char *end;
    int numerator = strtol(c, &end, base);

    if ((end - c < size - 1) && end[0] == '/') {
        char *start = end + 1;
        int denominator = strtol(start, &end, base);

        if (end - c == size) {
            *num = rational_number_new(numerator, denominator);
            return true;
        } else {
            return false;
        }
    } else if (end - c == size) {
        *num = rational_number_new(numerator, 1);
        return true;
    } else {
        return false;
    }
}

bool try_to_real_number(char *c, size_t size, int base, real_number_t *num) {
    char *end;
    double value = strtod(c, &end);

    if (end - c == size) {
        *num = real_number_new(value);
        return true;
    } else {
        return false;
    }
}