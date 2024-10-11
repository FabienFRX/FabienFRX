/*
** EPITECH PROJECT, 2023
** option_a
** File description:
** option for my_ls
*/

#include "include/my_ls.h"
#include "include/my.h"

int option_a(char *path)
{
    DIR *dir = opendir(path);
    struct dirent *file_folder;

    if (dir == NULL) {
        my_printf("ls: cannot access '%s'\n", path);
        return (84);
    }
    file_folder = readdir(dir);
    while (file_folder != NULL) {
        my_printf("%s  ", file_folder->d_name);
        file_folder = readdir(dir);
    }
    closedir(dir);
    my_putchar('\n');
    return (0);
}
