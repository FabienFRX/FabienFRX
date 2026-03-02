/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-projtester-fabien.fraixanet
** File description:
** bft_handle
*/

#include "../include/syn.h"

int is_access(char *pathing)
{
    int verify = 0;

    if ((verify = access(pathing, F_OK)) == 0) {
        if ((verify = access(pathing, X_OK)) != -1)
            return (1);
        return (0);
    }
    return (0);
}

void bft_handle(tree_t *tree, char *pathing)
{
    return;
}
