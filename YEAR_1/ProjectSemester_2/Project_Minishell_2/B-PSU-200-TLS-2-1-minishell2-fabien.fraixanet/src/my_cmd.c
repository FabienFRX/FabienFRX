/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_cmd
*/

#include "my_lib_incl.h"

void set_saved_cmd(char *str)
{
    char *save = NULL;

    save = str;
}

char *get_saved_cmd(void)
{
    char *save = NULL;

    return save;
}

char *my_cmd(char *str)
{
    char *save = NULL;

    if (str == NULL)
        return get_saved_cmd();
    set_saved_cmd(str);
    return save;
}
