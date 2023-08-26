#include "interpreter/interpreter.h"
#include "tokens/token.h"

#include <stdio.h>

int main(int argc, char **argv) {
    printf("C-Racket Version 0.1\n");
    printf("Press Ctrl+C to quit\n");

    interpreter_t interpreter = interpreter_new();

    if (argc >= 2) {
        for (int i = 1; i < argc; i += 1) {
            FILE *file = fopen(argv[i], "rb");

            if (file) {
                fseek(file, 0L, SEEK_END);
                int size = ftell(file);
                fseek(file, 0L, SEEK_SET);

                char *source = malloc(size);
                memset(source, 0, size);
                char *buf = malloc(size);

                while (fgets(buf, size, file)) {
                    strcat(source, buf);
                }
                printf("%s\n", source);
                interpreter_eval(&interpreter, source);

                free(source);
                fclose(file);
            }
        }
    }

    char input[2048];
    char *result;
    do {
        printf(">> ");

        result = fgets(input, 2048, stdin);

        ast_list_t nodes = interpreter_eval(&interpreter, result);

        for (unsigned int i = 0; i < nodes.len; i += 1) {
            ast_node_print(nodes.trees[i]);
        }

        ast_list_free(&nodes);
    } while (result != NULL);

    interpreter_free(&interpreter);

    return 0;
}
