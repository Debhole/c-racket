#include "ast_list.h"

ast_list_t ast_list_new() {
    int capacity = 8;
    ast_node_t **trees = malloc(sizeof(ast_node_t *) * capacity);

    return (ast_list_t) {
        .len = 0,
        .capacity = capacity,
        .trees = trees,
    };
}

ast_list_t ast_list_empty() {
    return (ast_list_t) {
        .len = 0,
        .capacity = 0,
        .trees = NULL
    };
}

void ast_list_free(ast_list_t *list) {
    for (int i = 0; i < list->len; i += 1) {
        ast_node_free(list->trees[i]);
    }

    free(list->trees);
    list->trees = NULL;
    list->len = 0;
    list->capacity = 0;
}

void ast_list_deform(ast_list_t *list, ast_node_t ***raw_trees, unsigned int *num_children) {
    ast_node_t **new_list = realloc(list->trees, sizeof(ast_node_t *) * list->len);

    if (new_list) {
        list->trees = new_list;
    }

    *raw_trees = list->trees;
    *num_children = list->len;
}

bool ast_list_push(ast_list_t *list, ast_node_t *tree) {
    if (list->len >= list->capacity) {
        list->capacity *= 2;
        ast_node_t **new_list = realloc(list->trees, sizeof(ast_node_t *) * list->capacity);

        if (new_list) {
            list->trees = new_list;
        } else {
            return false;
        }
    }

    list->trees[list->len] = tree;
    list->len += 1;

    return true;
}

bool ast_list_get(ast_list_t *list, unsigned int idx, ast_node_t **tree) {
    if (idx >= list->len) {
        return false;
    } else {
        (*tree) = list->trees[idx];
        return true;
    }
}