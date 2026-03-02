/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-projtester-fabien.fraixanet
** File description:
** sort_tree
*/

#include "../include/syn.h"

int swap_ptr(tree_t *current, tree_t *next)
{
    void *tmp = NULL;

    tmp = current->path;
    current->path = next->path;
    next->path = tmp;
    tmp = current->content;
    current->content = next->content;
    next->content = tmp;
}

void sort(tree_t *tree)
{
    int has_swap = 0;

    if (tree == NULL || tree->next == NULL)
        return;
    for (; tree != NULL && tree->next != NULL; tree = tree->next) {
        if (tree->path != NULL && strcmp(tree->path, tree->next->path) > 0)
            swap_ptr(tree, tree->next);
    }
    if (has_swap == 0)
        sort(tree);
}
