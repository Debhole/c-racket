#pragma once

#include "scanner/scanner.h"
#include "tokens/token.h"
#include "tokens/token_list.h"

#include "test_macros.h"

#include <string.h>

inline result_t test_scanner() {
    scanner_t s = scanner_new(
            ";Does this read basic delimiters correctly?\n"
            "([{}]) '`, ;Ignore the rest ()[\\a\n"
            "\"This is a string\"");

    token_list_t tokens = scanner_get_tokens(&s);

    // We expect 11 tokens
    // ( [ { } ] ) ' ` , "" EOF
    assert(tokens.len == 11);

    assert(tokens.tokens[0].type == TOKEN_LEFT_PAREN);
    assert(tokens.tokens[1].type == TOKEN_LEFT_BRACE);
    assert(tokens.tokens[2].type == TOKEN_LEFT_BRACKET);
    assert(tokens.tokens[3].type == TOKEN_RIGHT_BRACKET);
    assert(tokens.tokens[4].type == TOKEN_RIGHT_BRACE);
    assert(tokens.tokens[5].type == TOKEN_RIGHT_PAREN);
    assert(tokens.tokens[6].type == TOKEN_QUOTE);
    assert(tokens.tokens[7].type == TOKEN_QUASIQUOTE);
    assert(tokens.tokens[8].type == TOKEN_UNQUOTE);
    assert(tokens.tokens[9].type == TOKEN_STRING);
    assert(tokens.tokens[10].type == TOKEN_EOF);

    scanner_free(&s);
    token_list_free(&tokens);

    return SUCCESS;
}

inline result_t test_scanner_strings() {
    scanner_t s = scanner_new(
            "\"Hello \\\n"
            "Newline!\"");
    token_list_t tokens = scanner_get_tokens(&s);

    assert(tokens.len == 2);

    assert(tokens.tokens[0].type == TOKEN_STRING);
    assert_streq(tokens.tokens[0].data, "Hello Newline!");

    scanner_free(&s);
    token_list_free(&tokens);

    s = scanner_new(
            "\"Hi\\40There!\""
            "\"Does \\\"this\\\" show up \\\\correctly\\\\?\""
            "\"How about these? \\\'\\a\\b\\t\\n\\v\\f\\r\\\'\"");
    tokens = scanner_get_tokens(&s);

    assert(tokens.len == 4);

    assert(tokens.tokens[0].type == TOKEN_STRING);
    assert_streq(tokens.tokens[0].data, "Hi There!");
    assert_streq(tokens.tokens[1].data, "Does \"this\" show up \\correctly\\?");
    assert_streq(tokens.tokens[2].data, "How about these? \'\a\b\t\n\v\f\r\'");

    scanner_free(&s);
    token_list_free(&tokens);

    return SUCCESS;
}

inline result_t test_scanner_bools() {
    scanner_t s = scanner_new("#true(#t)#T #f'#false`#F");
    token_list_t tokens = scanner_get_tokens(&s);

    assert(tokens.len == 11);

    assert(tokens.tokens[0].type == TOKEN_BOOLEAN);
    assert(*(bool *) tokens.tokens[0].data == true);
    assert(*(bool *) tokens.tokens[2].data == true);
    assert(*(bool *) tokens.tokens[4].data == true);

    assert(tokens.tokens[5].type == TOKEN_BOOLEAN);
    assert(*(bool *) tokens.tokens[5].data == false);
    assert(*(bool *) tokens.tokens[7].data == false);
    assert(*(bool *) tokens.tokens[9].data == false);

    assert(tokens.tokens[1].type == TOKEN_LEFT_PAREN);
    assert(tokens.tokens[3].type == TOKEN_RIGHT_PAREN);
    assert(tokens.tokens[6].type == TOKEN_QUOTE);
    assert(tokens.tokens[8].type == TOKEN_QUASIQUOTE);

    scanner_free(&s);
    token_list_free(&tokens);

    return SUCCESS;
}

inline result_t test_scanner_datum() {
    scanner_t s = scanner_new("this_is_a_datum#12345 other junk...)");
    char buf[2048];

    assert(scanner_try_next_datum(&s, buf, sizeof buf));
    assert_streq(buf, "this_is_a_datum#12345");

    scanner_free(&s);

    s = scanner_new("this\\ is| a fuzzy datum| -not a part");
    assert(scanner_try_next_datum_fuzzy(&s, buf, sizeof buf));
    assert_streq(buf, "this is a fuzzy datum");

    return SUCCESS;
}

inline result_t test_scanner_numbers() {
    scanner_t s = scanner_new("1/2 123456789 0.5 129/132");
    token_list_t tokens = scanner_get_tokens(&s);

    assert(tokens.len == 5);
    assert(tokens.tokens[0].type == TOKEN_RATIONAL);
    assert(tokens.tokens[1].type == TOKEN_RATIONAL);
    assert(tokens.tokens[2].type == TOKEN_REAL);
    assert(tokens.tokens[3].type == TOKEN_RATIONAL);

    rational_number_t *rational = (rational_number_t *) tokens.tokens[0].data;
    assert(rational->numerator == 1);
    assert(rational->denominator == 2);

    rational = (rational_number_t *) tokens.tokens[1].data;
    assert(rational->numerator == 123456789);
    assert(rational->denominator == 1);

    rational = (rational_number_t *) tokens.tokens[3].data;
    assert(rational->numerator == 129);
    assert(rational->denominator == 132);

    real_number_t *real = (real_number_t *) tokens.tokens[2].data;
    assert(real->value == 0.5);

    scanner_free(&s);
    token_list_free(&tokens);

    return SUCCESS;
}

inline result_t test_scanner_symbols() {
    scanner_t s = scanner_new(
            "10/ abcd 50\\6 88\\ a\n"
            "|(multi \nline \nsymbol! \n100)|");
    token_list_t tokens = scanner_get_tokens(&s);

    assert(tokens.len == 6);
    assert(tokens.tokens[0].type == TOKEN_SYMBOL);
    assert(tokens.tokens[1].type == TOKEN_SYMBOL);
    assert(tokens.tokens[2].type == TOKEN_SYMBOL);
    assert(tokens.tokens[3].type == TOKEN_SYMBOL);
    assert(tokens.tokens[4].type == TOKEN_SYMBOL);

    assert_streq("10/", (char *)tokens.tokens[0].data);
    assert_streq("abcd", (char *)tokens.tokens[1].data);
    assert_streq("506", (char *)tokens.tokens[2].data);
    assert_streq("88 a", (char *)tokens.tokens[3].data);
    assert_streq("(multi \nline \nsymbol! \n100)", (char *)tokens.tokens[4].data);

    scanner_free(&s);
    token_list_free(&tokens);

    return SUCCESS;
}

inline result_t test_scanner_all() {
    scanner_t s = scanner_new(
            "(define (str\\ mul str int)\n"
            "  (cond\n"
            "    [(zero? int) \"\"]\n"
            "    [else\n"
            "      (string-append str (str\\ mul str (- int 1)))]))\n"
            "\n"
            "(str\\ mul \"hello!\" 3)"
            );
    token_list_t tokens = scanner_get_tokens(&s);

    assert(tokens.len == 40);

    // Line 1 - 0-6 - "(define (str\ mul str int)"
    assert(tokens.tokens[0].type == TOKEN_LEFT_PAREN);
    assert(tokens.tokens[2].type == TOKEN_LEFT_PAREN);
    assert(tokens.tokens[6].type == TOKEN_RIGHT_PAREN);

    assert_symeq(tokens.tokens[1], "define");
    assert_symeq(tokens.tokens[3], "str mul");
    assert_symeq(tokens.tokens[4], "str");
    assert_symeq(tokens.tokens[5], "int");

    // Line 2 - 7-8 - "(cond"
    assert(tokens.tokens[7].type == TOKEN_LEFT_PAREN);
    assert_symeq(tokens.tokens[8], "cond");

    // Line 3 - 9-15 - "[(zero? int) ""]"
    assert(tokens.tokens[9].type == TOKEN_LEFT_BRACE);
    assert(tokens.tokens[10].type == TOKEN_LEFT_PAREN);
    assert(tokens.tokens[13].type == TOKEN_RIGHT_PAREN);
    assert(tokens.tokens[15].type == TOKEN_RIGHT_BRACE);

    assert_symeq(tokens.tokens[11], "zero?");
    assert_symeq(tokens.tokens[12], "int");

    assert(tokens.tokens[14].type == TOKEN_STRING);
    assert_streq("", (char *)tokens.tokens[14].data);

    // Line 4 - 16-17 - "[else"
    assert(tokens.tokens[16].type == TOKEN_LEFT_BRACE);
    assert_symeq(tokens.tokens[17], "else");

    // Line 5 - 18-33 - "(string-append str (str\ mul str (- int 1)))]))"
    assert(tokens.tokens[18].type == TOKEN_LEFT_PAREN);
    assert_symeq(tokens.tokens[19], "string-append");
    assert_symeq(tokens.tokens[20], "str");
    assert(tokens.tokens[21].type == TOKEN_LEFT_PAREN);
    assert_symeq(tokens.tokens[22], "str mul");
    assert_symeq(tokens.tokens[23], "str");
    assert(tokens.tokens[24].type == TOKEN_LEFT_PAREN);
    assert_symeq(tokens.tokens[25], "-");
    assert_symeq(tokens.tokens[26], "int");

    assert(tokens.tokens[27].type == TOKEN_RATIONAL);
    assert(((rational_number_t *)tokens.tokens[27].data)->numerator == 1);
    assert(((rational_number_t *)tokens.tokens[27].data)->denominator == 1);

    assert(tokens.tokens[28].type == TOKEN_RIGHT_PAREN);
    assert(tokens.tokens[29].type == TOKEN_RIGHT_PAREN);
    assert(tokens.tokens[30].type == TOKEN_RIGHT_PAREN);
    assert(tokens.tokens[31].type == TOKEN_RIGHT_BRACE);
    assert(tokens.tokens[32].type == TOKEN_RIGHT_PAREN);
    assert(tokens.tokens[33].type == TOKEN_RIGHT_PAREN);

    // Line 7 - 34-39 - "(str\ mul "hello!" 3)"
    assert(tokens.tokens[34].type == TOKEN_LEFT_PAREN);
    assert_symeq(tokens.tokens[35], "str mul");

    assert(tokens.tokens[36].type == TOKEN_STRING);
    assert_streq("hello!", (char *)tokens.tokens[36].data);

    assert(tokens.tokens[37].type == TOKEN_RATIONAL);
    assert(((rational_number_t *)tokens.tokens[37].data)->numerator == 3);
    assert(((rational_number_t *)tokens.tokens[37].data)->denominator == 1);

    assert(tokens.tokens[38].type == TOKEN_RIGHT_PAREN);
    assert(tokens.tokens[39].type == TOKEN_EOF);

    return SUCCESS;
}