#include "keyword.h"
#include <string.h>

bool is_keyword(const char *c) {
    const char *keywords[7] = {
            "define",
            "define_struct",
            "cond",
            "else",
            "if",
            "and",
            "or",
    };

    for (int i = 0; i < 7; i += 1) {
        if (strcmp(c, keywords[i]) == 0) {
            return true;
        }
    }

    return false;
}