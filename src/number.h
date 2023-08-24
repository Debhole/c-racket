#pragma once

#include <stdbool.h>
#include <stdlib.h>

bool try_to_integer(char *c, size_t size, int base, int *num);

bool try_to_double(char *c, size_t size, double *num);
