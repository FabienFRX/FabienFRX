/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-projtester-fabien.fraixanet
** File description:
** free functions
*/

#include "../include/syn.h"

void free_array(char **arr)
{
    for (int i = 0; arr[i] != NULL; i++) {
        if (arr[i]) {
            free(arr[i]);
        }
    }
    if (arr)
        free(arr);
}

void free_tree(tree_t *tree)
{
    tree_t *tmp = NULL;

    printf("free tree\n");
    while (tree != NULL) {
        tmp = tree;
        tree = tree->next;
        printf("ptr: %p, path%s, content: %p\n", tmp, tmp->path, tmp->content);
        if (tmp->path != NULL) {
            printf("free path...\n");
            free(tmp->path);
        }
        if (tmp->content != NULL) {
            printf("free content...");
            free_array(tmp->content);
        }
        printf("test\n");
        free(tmp);
        printf("test2\n");
    }
}
