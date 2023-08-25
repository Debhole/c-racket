#include "ast_list.h"

ast_list_t ast_list_new(void) {
    int capacity = 8;
    ast_node_t **trees = malloc(sizeof(ast_node_t *) * capacity);

    return (ast_list_t){
            .len = 0,
            .capacity = capacity,
            .trees = trees,
    };
}

ast_list_t ast_list_clone(ast_list_t *list) {
    ast_list_t clone = ast_list_new();

    for (unsigned int i = 0; i < list->len; i += 1) {
        ast_node_t *node = ast_node_clone(list->trees[i]);

        ast_list_push(&clone, node);
    }

    return clone;
}

void ast_list_free(ast_list_t *list) {
    for (unsigned int i = 0; i < list->len; i += 1) {
        if (list->trees[i]) {
            ast_node_free(list->trees[i]);
        }
    }

    free(list->trees);
    list->trees = NULL;
    list->len = 0;
    list->capacity = 0;
}

void ast_list_deform(ast_list_t *list, ast_node_t ***raw_trees, unsigned int *num_children) {
    if (list->len > 0) {
        ast_node_t **tmp_list = realloc(list->trees, sizeof(ast_node_t *) * list->len);

        if (tmp_list) {
            list->trees = tmp_list;
        }
    } else {
        free(list->trees);
        list->trees = NULL;
    }

    *raw_trees = list->trees;
    *num_children = list->len;

    list->trees = NULL;
    list->len = 0;
    list->capacity = 0;
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

bool ast_list_get_clone(ast_list_t *list, unsigned int idx, ast_node_t **tree) {
    if (!ast_list_get(list, idx, tree)) {
        return false;
    } else {
        (*tree) = ast_node_clone(*tree);
        return true;
    }
}
