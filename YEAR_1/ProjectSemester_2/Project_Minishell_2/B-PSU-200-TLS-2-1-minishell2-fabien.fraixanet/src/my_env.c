/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_env
*/

#include "my_lib_incl.h"

void print_env(char **env)
{
    int i = 0;

    while (env[i] != NULL) {
        my_putstr(env[i], 1);
        write(1, "\n", 1);
        i++;
    }
}

char **my_env(char **cmd, char **env)
{
    (void) cmd;
    print_env(env);
    return env;
}
