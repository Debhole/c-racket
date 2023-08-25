#include "definition.h"

definition_t definition_new(const char *name, ast_node_t *value) {
    size_t size = strlen(name) + 1;
    char *owned_name = malloc(size);
    strcpy(owned_name, name);

    ast_node_t *owned_value = ast_node_clone(value);
    return (definition_t) {
        .name = owned_name,
        .value = owned_value,
    };
}

void definition_free(definition_t *definition) {
    free(definition->name);
    ast_node_free(definition->value);

    definition->name = NULL;
    definition->value = NULL;
}
