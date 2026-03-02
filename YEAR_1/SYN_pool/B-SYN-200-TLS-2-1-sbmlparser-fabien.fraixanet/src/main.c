/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-sbmlparser-fabien.fraixanet
** File description:
** main
*/

#include "../include/syn.h"

int options(int argc, char **argv)
{
    char *file = NULL;

    if (argc == 3 && my_strcmp(argv[1], "-i") == 0)
        option_i();
        return (0);
    if (argc == 5 && my_strcmp(argv[1], "-e") == 0)
        option_e();
        return (0);
    if (argc == 5 && my_strcmp(argv[1], "-json") == 0)
        option_json();
        return (0);
    file = file_reader(file);
    if (file == NULL)
        return (84);
    return (0);
}

int main(int argc, char **argv)
{
    char *file = NULL;

    if (argc < 2)
        return (84);
    if (argc == 2 && my_strcmp(argv[1], "-h") == 0)
        display_help();
        return (0);
    if (!file)
        return (84);
    if (!options)
        return (84);
    if (argc == options(argc, argv))
        return (0);
    else
        return (84);
    return (0);
}
