/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** single_old
*/

#include "my_lib_incl.h"

void set_old(char *str)
{
    static char *save = NULL;

    save = str;
}

char *get_old(void)
{
    static char *save = NULL;

    return save;
}

char *single_old(char *str)
{
    if (str == NULL)
        return get_old();
    set_old(str);
    return get_old();
}
