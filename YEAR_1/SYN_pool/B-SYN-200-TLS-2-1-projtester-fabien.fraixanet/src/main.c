/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-projtester-fabien.fraixanet
** File description:
** main
*/

#include "../include/syn.h"

int main(int argc, char **argv)
{
    tree_t tree = {0};
    char *path = strdup(argv[1]);

    if (argc < 2 || argc > 3) {
        display_help();
        return (84);
    }
    if (path[strlen(path) - 1] == '/')
        path[strlen(path) - 1] = '\0';
    tree_handle(path, &tree);
    if (&tree == NULL || tree.next == NULL)
        return (84);
    sort(&tree);
    if (argc == 2) {
        display_tree(path, &tree);
    }
    if (argc == 3) {
        command_handle(&tree, path, argv[2]);
    }
    return (0);
}
