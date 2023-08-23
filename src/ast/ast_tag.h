#pragma once

typedef enum ast_tag_t {
    TAG_EXPRESSION,
    TAG_DEFINITION,
    TAG_RATIONAL,
    TAG_REAL,
    TAG_STRING,
    TAG_BOOLEAN,
    TAG_KEYWORD,
    TAG_SYMBOL,
    TAG_COND_CLAUSE,
} ast_tag_t;
