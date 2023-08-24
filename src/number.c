#include "number.h"

#include <ctype.h>
#include <stdlib.h>

bool try_to_integer(char *c, size_t size, int base, int *num) {
    char *end;
    int value = strtol(c, &end, base);

    size_t loc = (size_t) (end - c);

    if (loc == size) {
        *num = value;
        return true;
    } else {
        return false;
    }
}

bool try_to_double(char *c, size_t size, double *num) {
    char *end;
    double value = strtod(c, &end);

    size_t loc = (size_t) (end - c);

    if (loc == size) {
        *num = value;
        return true;
    } else {
        return false;
    }
}
