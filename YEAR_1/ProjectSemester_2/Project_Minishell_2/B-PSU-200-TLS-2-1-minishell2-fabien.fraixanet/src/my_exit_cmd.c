/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_exit_cmd
*/

#include "my_lib_incl.h"

void free_resources(char **cmd, char **env)
{
    my_free(cmd);
    my_free(env);
}

char **my_exit(char **cmd, char **env)
{
    free_resources(cmd, env);
    exit(0);
    return env;
}
