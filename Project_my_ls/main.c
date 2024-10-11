/*
** EPITECH PROJECT, 2023
** main
** File description:
** test main
*/

#include "include/my_ls.h"
#include "include/my.h"

int list_options(const char *path, int recurs, int hide, int long_form)
{
    process_dir(path, recurs, hide, long_form);
    return (0);
}

int different_option(char option, int *hide, int *long_form, int *recurs)
{
    switch (option) {
    case 'a':
        *hide = 1;
        break;
    case 'l':
        *long_form = 1;
        break;
    case 'R':
        *recurs = 1;
        break;
    default:
        my_printf("not good options %c\n", option);
        return (84);
    }
    return (0);
}

int main(int argc, char **argv)
{
    const char *path = ".";
    int recurs = 0;
    int hide = 0;
    int long_form = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j] != '\0'; j++) {
                different_option(argv[i][j], &hide, &long_form, &recurs);
            }
        } else {
            path = argv[i];
        }
    }
    list_options(path, recurs, hide, long_form);
    return (0);
}
