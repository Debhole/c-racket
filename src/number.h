#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef enum number_type_t {
    NUMBER_RATIONAL,
    NUMBER_REAL,
} number_type_t;

typedef struct rational_number_t {
    int numerator;
    int denominator;
} rational_number_t;

rational_number_t rational_number_new(int numerator, int denominator);

typedef struct real_number_t {
    double value;
} real_number_t;

real_number_t real_number_new(double value);

bool try_to_rational_number(char *c, size_t size, int base, rational_number_t *num);

bool try_to_real_number(char *c, size_t size, int base, real_number_t *num);