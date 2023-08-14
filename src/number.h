#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef enum number_type_t {
    NUMBER_RATIONAL,
    NUMBER_REAL,
    NUMBER_COMPLEX,
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

typedef struct complex_number_t {
    double real;
    double complex;
} complex_number_t;

complex_number_t complex_number_new(double real, double complex);

bool try_to_rational_number(char *c, size_t size, rational_number_t *num);

bool try_to_real_number(char *c, size_t size, real_number_t *num);

bool try_to_complex_number(char *c, size_t size, complex_number_t *num);