/*
** EPITECH PROJECT, 2023
** my_ls
** File description:
** function wich remake ls
*/

#include "include/my_ls.h"
#include "include/my.h"

int my_ls(char *path)
{
    DIR *dir = opendir(path);
    struct dirent *file_folder;

    if (dir == NULL) {
        my_printf("ls: cannot access '%s'\n", path);
        return (84);
    }
    file_folder = readdir(dir);
    while (file_folder != NULL) {
        if (file_folder->d_name[0] != '.') {
            my_printf("%s  ", file_folder->d_name);
        }
        file_folder = readdir(dir);
    }
    closedir(dir);
    my_putchar('\n');
    return (0);
}
