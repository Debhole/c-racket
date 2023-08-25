#include "interpreter.h"
#include "ast/ast_builder.h"
#include "scanner/scanner.h"

interpreter_t interpreter_new(void) {
    function_book_t function_book = function_book_new();
    function_book_push(&function_book, primitive_function_new("boolean?", 1, true, &prim_isboolean));
    function_book_push(&function_book, primitive_function_new("number?", 1, true, &prim_isnumber));
    function_book_push(&function_book, primitive_function_new("symbol?", 1, true, &prim_issymbol));
    function_book_push(&function_book, primitive_function_new("string?", 1, true, &prim_isstring));

    function_book_push(&function_book, primitive_function_new("+", 0, false, &prim_add));
    function_book_push(&function_book, primitive_function_new("-", 1, false, &prim_sub));
    function_book_push(&function_book, primitive_function_new("*", 0, false, &prim_mul));

    function_book_push(&function_book, primitive_function_new("string-append", 0, false, &prim_string_append));

    return (interpreter_t){
            .function_book = function_book,
    };
}

void interpreter_free(interpreter_t *interpreter) {
    function_book_free(&interpreter->function_book);
}

ast_list_t interpreter_eval(interpreter_t *interpreter, const char *source) {
    scanner_t s = scanner_new(source);
    token_list_t tokens = scanner_get_tokens(&s);
    ast_builder_t b = ast_builder_new(&tokens);

    ast_list_t trees = ast_builder_get_trees(&b);
    ast_list_t evals = ast_list_new();

    for (unsigned int i = 0; i < trees.len; i += 1) {
        ast_node_t *tree;
        if (!ast_list_get(&trees, i, &tree)) {
            break;
        } else {
            ast_node_t *eval = interpreter_eval_node(interpreter, tree);
            if (eval) {
                ast_list_push(&evals, eval);
            }
        }
    }

    scanner_free(&s);
    ast_builder_free(&b);
    ast_list_free(&trees);

    return evals;
}

ast_node_t *interpreter_eval_node(interpreter_t *interpreter, ast_node_t *node) {
    switch (node->tag) {
        case TAG_EXPRESSION: {
            char *fn = (char *) node->data;

            return interpreter_eval_fn(interpreter, fn, node->children, node->num_children);
        }

        case TAG_COND_CLAUSE:
        case TAG_DEFINITION:
            return NULL;

        case TAG_INTEGER:
        case TAG_DOUBLE:
        case TAG_STRING:
        case TAG_BOOLEAN:
        case TAG_KEYWORD:
        case TAG_SYMBOL:
            return ast_node_clone(node);

        default:
            return NULL;
    }
}

ast_node_t *interpreter_eval_fn(interpreter_t *interpreter, const char *fn, ast_node_t **args, unsigned int num_args) {
    ast_list_t eval_args = ast_list_new();
    for (unsigned int i = 0; i < num_args; i += 1) {
        ast_list_push(&eval_args, interpreter_eval_node(interpreter, args[i]));
    }

    primitive_function_t pfn;
    ast_node_t *eval_node = NULL;
    if (function_book_contains(&interpreter->function_book, fn, &pfn)) {
        bool arity_matches = ((!pfn.precise_arity && eval_args.len >= pfn.arity) || eval_args.len == pfn.arity);

        if (arity_matches) {
            eval_node = pfn.fn(&eval_args);
        }
    }

    ast_list_free(&eval_args);
    return eval_node;
}
