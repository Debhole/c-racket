#include "interpreter/interpreter.h"
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

    interpreter_t interpreter = interpreter_new();

    do {
        printf(">> ");

        result = fgets(input, 2048, stdin);

        ast_list_t nodes = interpreter_eval(&interpreter, result);

        for (unsigned int i = 0; i < nodes.len; i += 1) {
            ast_node_print(nodes.trees[i]);
        }
    } while (result != NULL);

    return 0;
}
