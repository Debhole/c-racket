#pragma once

#include "tokens/token_list.h"

#include <stdbool.h>

typedef struct scanner_t {
    char *source;
    size_t len;
    unsigned int loc;
    unsigned int line;
} scanner_t;

scanner_t scanner_new(const char *source);
void scanner_free(scanner_t *s);

/// Get all tokens from the scanner
/// \param s The scanner
/// \return A list of tokens from the scanner
token_list_t scanner_get_tokens(scanner_t *s);

/// Attempts to parse the next token from the source file. Modifies
/// the token_t pointer only if it found another token.
/// \param s A pointer to the scanner to use
/// \param t A pointer to the token to set
/// \return True if there was another token, False if there was not
bool scanner_try_next_token(scanner_t *s, token_t *t);

bool scanner_try_next_string(scanner_t *s, token_t *t);

bool scanner_try_next_hash_delimited(scanner_t *s, token_t *t);

bool scanner_try_next_sym_num(scanner_t *s, token_t *t);

bool scanner_try_next_datum(scanner_t *s, char * c, size_t size);

bool scanner_try_next_datum_fuzzy(scanner_t *s, char * c, size_t size);

bool scanner_at_end(scanner_t *s);

char scanner_peek(scanner_t *s);

char scanner_peek_two(scanner_t *s);

/// Advances to the next character
/// \param s A pointer to the scanner to use
/// \return The next character
char scanner_advance(scanner_t *s);

bool scanner_str_advance(scanner_t *s, char *c);

void scanner_jump_endl(scanner_t *s);

/// Returns whether the given character is a delimiter or not. The following
/// characters are delimiters: ( ) [ ] { } , ' ` " ; and whitespace and BOM character
/// \param c The character to compare
/// \return Whether or not the given character is a delimiter
bool is_delimiter(char c);