/*
** EPITECH PROJECT, 2023
** option_d
** File description:
** option for my_ls
*/

#include "include/my_ls.h"
#include "include/my.h"

int option_d(char *path)
{
    DIR *dir = opendir(path);

    if (dir == NULL) {
        my_printf("ls: cannot access '%s'\n", path);
        return (84);
    }
    if (path == NULL) {
        my_printf("error");
        return (84);
    }
    my_printf("%s\n", path);
    closedir(dir);
    return (0);
}
