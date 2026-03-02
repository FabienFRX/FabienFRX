/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-FASTAtools-fabien.fraixanet
** File description:
** main
*/

#include "syn.h"

int error_handling(int argc, char **argv)
{
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        display_help();
        return (84);
    }
    if (argc != 1)
        return (84);
    return (0);
}

int main(int argc, char **argv)
{
    if (error_handling(argc, argv) == 84)
        return (84);
    loop_till_end();
    return (0);
}
