/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-projtester-fabien.fraixanet
** File description:
** main
*/

#include "../include/syn.h"

int main(int argc, char **argv)
{
    tree_t tree;

    if (argc < 2 || argc > 4) {
        display_help();
        return (84);
    }
    tree_handle(argv[1], &tree);
    sort(&tree);
    if (argc == 2) {
        display_tree(argv[1], &tree);
    }
    if (argc == 3) {
        command_handle(&tree, argv[1], argv[2]);
    }
    return (0);
}
