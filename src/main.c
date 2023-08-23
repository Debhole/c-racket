#include "scanner/scanner.h"
#include "tokens/token.h"
#include "tokens/token_list.h"

#include <stdio.h>

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("C-Racket BSL Version 0.1\n");
    printf("Press Ctrl+C to quit\n");

    char input[2048];
    char *result;

    scanner_t s;

    do {
        printf(">> ");

        result = fgets(input, 2048, stdin);

        s = scanner_new(input);
        token_list_t list = scanner_get_tokens(&s);

        token_t tmp;
        for (unsigned int i = 0; i < list.len; i++) {
            token_list_get(&list, i, &tmp);
            token_print(&tmp);
        }

        scanner_free(&s);
        token_list_free(&list);
    } while (result != NULL);

    return 0;
}
