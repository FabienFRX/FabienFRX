/*
** EPITECH PROJECT, 2025
** Paradigms Seminar
** File description:
** Exercise 08
*/

#ifndef TREE_H
    #define TREE_H

    #include "list.h"

/*
** Types
*/

/* Tree */

typedef struct tree_s {
    void *data;
    list_t *children;
} tree_t;

/* Misc */

typedef void (*dump_func_t)(void *data);

/* Generic Container */

typedef bool (*push_func_t)(void *container, void *data);
typedef void *(*pop_func_t)(void *container);

typedef struct container_s {
    void *container;
    push_func_t push_func;
    pop_func_t pop_func;
} container_t;

/*
** Functions
*/

/* Informations */

bool tree_is_empty(tree_t *tree);
void tree_node_dump(tree_t *node, dump_func_t dump_func);

/* Modifications */

bool init_tree(tree_t **tree_ptr, void *data);
tree_t *tree_add_child(tree_t *node, void *data);
bool tree_destroy(tree_t **tree_ptr);

/* Access */

void tree_traversal(tree_t *tree, container_t *container,
    dump_func_t dump_func);

#endif
