#include "ast_builder.h"
#include "ast_list.h"

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

ast_list_t ast_builder_get_trees(ast_builder_t *b) {
    ast_list_t trees = ast_list_new();

    ast_node_t *tree;
    while ((tree = ast_builder_next_tree(b))) {
        if (!ast_list_push(&trees, tree)) {
            // Handle error if fails to push tree
        }
    }

    return trees;
}

ast_node_t *ast_builder_next_tree(ast_builder_t *b) {
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
            case TOKEN_RIGHT_BRACE:
            case TOKEN_RIGHT_BRACKET:
                return NULL;

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
            case TOKEN_ERROR:
                return NULL;
        }
    } else {
        return NULL;
    }
}

ast_node_t *ast_builder_next_expression(ast_builder_t *b, token_type_t end) {
    token_t expr_head;
    if (ast_builder_advance(b, &expr_head) && (expr_head.type == TOKEN_SYMBOL || expr_head.type == TOKEN_KEYWORD)) {

        ast_list_t children = ast_list_new();

        token_t next;
        while (ast_builder_peek(b, &next) && next.type != end) {
            ast_node_t *tree = ast_builder_next_tree(b);

            if (!tree) {
                return NULL;
            } else {
                ast_list_push(&children, tree);
            }
        }

        if (next.type == end) {
            ast_builder_advance(b, &next);
        }

        ast_node_t **raw_children;
        unsigned int num_children;
        ast_list_deform(&children, &raw_children, &num_children);

        return ast_node_new(TAG_EXPRESSION, expr_head.data, children.len, children.trees);
    }
}

bool ast_builder_advance(ast_builder_t *b, token_t *t) {
    bool result = ast_builder_peek(b, t);
    b->loc += 1;

    return result;
}

bool ast_builder_peek(ast_builder_t *b, token_t *t) {
    return token_list_get(b->tokens, b->loc, t) && t->type != TOKEN_EOF;
}