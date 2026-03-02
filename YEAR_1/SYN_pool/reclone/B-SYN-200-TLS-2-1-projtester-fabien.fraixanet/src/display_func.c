/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-projtester-fabien.fraixanet
** File description:
** display_help
*/

#include "../include/syn.h"

void display_help(void)
{
    printf("USAGE\n");
    printf("    ./projTester TRD \\[BFT\\] [outputFile]\n");
    printf("DESCRIPTION\n");
    printf("    TRD \t    root directory of all the tests\n");
    printf("    BFT \t    binary file to be tested\n");
    printf("    outputFile \t    file in which the ressult is printed\n");
}

static void display_item(char *directory, tree_t *node)
{
    int sub_level = 0;
    int last = 0;

    if (node->path == NULL)
        return;
    for (int i = strlen(directory); node->path[i] != '\0'; i++)
        if (node->path[i] == '/') {
            sub_level++;
            last = i;
        }
    for (int i = 0; i < sub_level; i++)
        write(1, "-----", 5);
    for (int i = last + 1; node->path[i] != '\0'; i++)
        putchar(node->path[i]);
    putchar('\n');
}

void display_tree(char *directory, tree_t *tree)
{
    printf("%s\n", directory);
    for (; tree != NULL; tree = tree->next) {
        display_item(directory, tree);
    }
}
