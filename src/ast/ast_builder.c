#include "ast_builder.h"

ast_builder_t ast_builder_new(token_list_t *tokens) {
    return (ast_builder_t){
            .tokens = tokens,
            .loc = 0,
    };
}

void ast_builder_free(ast_builder_t *b) {
    token_list_free(b->tokens);
    b->loc = 0;
}

ast_node_t *ast_builder_build_tree(ast_builder_t *b) {
    token_t curr;

    if (ast_builder_advance(b, &curr)) {
        switch (curr.type) {
            case TOKEN_LEFT_PAREN:
                return ast_builder_next_expression(b, TOKEN_RIGHT_PAREN);
            case TOKEN_LEFT_BRACE:
                return ast_builder_next_expression(b, TOKEN_RIGHT_BRACE);
            case TOKEN_LEFT_BRACKET:
                return ast_builder_next_expression(b, TOKEN_RIGHT_BRACKET);

            // Reading one of these tokens at the start of a node means there
            // are mismatched/extra closing groups
            case TOKEN_RIGHT_PAREN:
                break;
            case TOKEN_RIGHT_BRACE:
                break;
            case TOKEN_RIGHT_BRACKET:
                break;

            case TOKEN_QUOTE:
                break;
            case TOKEN_QUASIQUOTE:
                break;
            case TOKEN_UNQUOTE:
                break;

            case TOKEN_STRING:
                return ast_node_new(TAG_STRING, curr.data, 0, NULL);
            case TOKEN_RATIONAL:
                return ast_node_new(TAG_RATIONAL, curr.data, 0, NULL);
            case TOKEN_REAL:
                return ast_node_new(TAG_REAL, curr.data, 0, NULL);
            case TOKEN_BOOLEAN:
                return ast_node_new(TAG_BOOLEAN, curr.data, 0, NULL);
            case TOKEN_SYMBOL:
                return ast_node_new(TAG_SYMBOL, curr.data, 0, NULL);
            case TOKEN_KEYWORD:
                return ast_node_new(TAG_KEYWORD, curr.data, 0, NULL);

            case TOKEN_EOF:
                break;
            case TOKEN_ERROR:
                break;
        }
    } else {
        return NULL;
    }
}

ast_node_t *ast_builder_next_expression(ast_builder_t *b, token_type_t end) {
    token_t expr_head;
    if (ast_builder_advance(b, &expr_head) && (expr_head.type == TOKEN_SYMBOL || expr_head.type == TOKEN_KEYWORD)) {

        int capacity = 8;
        unsigned int num_children = 0;
        ast_node_t **children = malloc(sizeof(ast_node_t *) * capacity);

        token_t child;
        while (ast_builder_peek(b, &child) && child.type != end) {
            if (num_children >= capacity) {
                capacity *= 2;
                ast_node_t **new_children = realloc(children, sizeof(ast_node_t *) * capacity);

                if (new_children) {
                    children = new_children;
                }
            }
            children[num_children] = ast_builder_build_tree(b);
            num_children += 1;
        }

        ast_node_t **exact_size_children = realloc(children, sizeof(ast_node_t *) * num_children);
        if (exact_size_children) {
            children = exact_size_children;
        }

        return ast_node_new(TAG_EXPRESSION, expr_head.data, num_children, children);
    }
}

bool ast_builder_advance(ast_builder_t *b, token_t *t) {
    bool result = token_list_get(b->tokens, b->loc, t) && t->type != TOKEN_EOF;
    b->loc += 1;

    return result;
}

bool ast_builder_peek(ast_builder_t *b, token_t *t) {
    return token_list_get(b->tokens, b->loc, t) && t->type != TOKEN_EOF;
}