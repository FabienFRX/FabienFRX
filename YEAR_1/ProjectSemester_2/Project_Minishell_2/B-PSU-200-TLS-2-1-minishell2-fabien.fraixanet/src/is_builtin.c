/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** is_builtin
*/

#include "my_lib_incl.h"

int check_builtin(char **built_in, char *cmd)
{
    for (int i = 0; built_in[i]; i++) {
        if (my_strcmp(built_in[i], cmd) == 0)
            return 1;
    }
    return 0;
}

int is_builtin(char *cmd)
{
    char *built_in[] = {"setenv", "unsetenv", "env", "cd", "exit", NULL};

    return check_builtin(built_in, cmd);
}
