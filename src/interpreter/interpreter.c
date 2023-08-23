#include "interpreter.h"
#include "ast/ast_builder.h"
#include "scanner/scanner.h"

interpreter_t interpreter_new(void) {
    return (interpreter_t){
            .tmp = 0,
    };
}

void interpreter_free(interpreter_t *interpreter) {
    interpreter->tmp = 0;
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
            char *fn = (char *)node->data;

            return interpreter_eval_fn(interpreter, fn, node->children, node->num_children);
        }

        case TAG_COND_CLAUSE:
        case TAG_DEFINITION:
            return NULL;

        case TAG_RATIONAL:
        case TAG_REAL:
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

    ast_node_t *eval_node;
    if (strcmp(fn, "boolean?") == 0) {
        if (eval_args.len == 1) {
            ast_node_t *ref_node;
            bool result = ast_list_get(&eval_args, 0, &ref_node);
            bool eval = (result && ref_node->tag == TAG_BOOLEAN) ? true : false;
            eval_node = ast_node_new(TAG_BOOLEAN, &eval, sizeof(bool), 0, NULL);
        } else {
            eval_node = NULL;
        }
    } else if(strcmp(fn, "number?") == 0) {
        if (eval_args.len == 1) {
            ast_node_t *ref_node;
            bool result = ast_list_get(&eval_args, 0, &ref_node);
            bool eval = (result && (ref_node->tag == TAG_RATIONAL || ref_node->tag == TAG_REAL)) ? true : false;
            eval_node = ast_node_new(TAG_BOOLEAN, &eval, sizeof(bool), 0, NULL);
        } else {
            eval_node = NULL;
        }
    } else if(strcmp(fn, "symbol?") == 0) {
        if (eval_args.len == 1) {
            ast_node_t *ref_node;
            bool result = ast_list_get(&eval_args, 0, &ref_node);
            bool eval = (result && ref_node->tag == TAG_SYMBOL) ? true : false;
            eval_node = ast_node_new(TAG_BOOLEAN, &eval, sizeof(bool), 0, NULL);
        } else {
            eval_node = NULL;
        }
    } else {
        eval_node = NULL;
    }

    ast_list_free(&eval_args);
    return eval_node;
}
