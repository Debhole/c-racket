#include "token_type.h"

const char *token_type_to_string(token_type_t type) {
    switch (type) {
        case TOKEN_LEFT_PAREN: return "Left Parenthesis";
        case TOKEN_RIGHT_PAREN:
            return "Right Parenthesis";
        case TOKEN_LEFT_BRACE:
            return "Left Brace";
        case TOKEN_RIGHT_BRACE:
            return "Right Brace";
        case TOKEN_LEFT_BRACKET:
            return "Left Bracket";
        case TOKEN_RIGHT_BRACKET:
            return "Right Bracket";
        case TOKEN_QUOTE:
            return "Quote";
        case TOKEN_QUASIQUOTE:
            return "Quasiquote";
        case TOKEN_UNQUOTE:
            return "Unquote";
        case TOKEN_STRING:
            return "String";
        case TOKEN_RATIONAL:
            return "Rational";
        case TOKEN_REAL:
            return "Real";
        case TOKEN_BOOLEAN:
            return "Boolean";
        case TOKEN_SYMBOL:
            return "Symbol";
        case TOKEN_KEYWORD:
            return "Keyword";
        case TOKEN_EOF:
            return "End of File";
        case TOKEN_ERROR:
            return "Error";
    }
}