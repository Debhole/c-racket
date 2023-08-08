#include <stdio.h>

int main() {
    printf("C-Racket BSL Version 0.1\n");
    printf("Press Ctrl+C to quit\n");

    char input[2048];
    char *result;

    do {
        printf(">> ");

        result = fgets(input, 2048, stdin);

        printf("%s", input);
    } while (result != NULL);

    return 0;
}
