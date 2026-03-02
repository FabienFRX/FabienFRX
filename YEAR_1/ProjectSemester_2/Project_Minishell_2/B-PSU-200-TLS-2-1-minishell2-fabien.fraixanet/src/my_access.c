/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_access
*/

#include "my_lib_incl.h"

void free_strtab(char **tab)
{
    int i = 0;

    while (tab[i] != NULL) {
        free(tab[i]);
        i++;
    }
    free(tab);
}

int check_path_access(char **temp, char *str)
{
    char *pwd = NULL;
    int i = 0;

    while (temp[i] != NULL) {
        pwd = my_strcat_path(temp[i], str);
        if (pwd == NULL) {
            free_strtab(temp);
            return (-1);
        }
        if (access(pwd, F_OK) != -1 && open(pwd, O_RDONLY) != -1) {
            my_cmd(pwd);
            free(pwd);
            free_strtab(temp);
            return (0);
        }
        free(pwd);
        i++;
    }
    free_strtab(temp);
    return (-1);
}

int my_access(char *str, char *path)
{
    char **temp = NULL;

    if (path != NULL) {
        temp = my_strtab(path + 5, ':');
        if (temp == NULL)
            return (-1);
        return check_path_access(temp, str);
    }
    return (-1);
}
