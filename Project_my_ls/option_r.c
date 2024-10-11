/*
** EPITECH PROJECT, 2023
** option_R
** File description:
** display all path and all files in their
*/
#include "include/my_ls.h"
#include "include/my.h"

int option_r(char *path)
{
    for (int i = 0; path != NULL; i++) {
        my_printf("%s\n", path);
        return (my_ls(path));
    }
    return (0);
}
