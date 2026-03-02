/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** single_env
*/

#include "my_lib_incl.h"

void set_env(char **env)
{
    static char **str = NULL;

    str = env;
}

char **get_env(void)
{
    static char **str = NULL;

    return str;
}

char **single_env(char **env)
{
    if (env == NULL)
        return get_env();
    set_env(env);
    return get_env();
}
