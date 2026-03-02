/*
** EPITECH PROJECT, 2024
** B-PSU-200-TLS-2-1-minishell1-fabien.fraixanet
** File description:
** env
*/

#include "../include/my.h"

void my_env(char **env)
{
    for (int i = 0; env != NULL; i++) {
        my_putstr(env[i]);
        my_putchar('\n');
    }
}

const char *get_env_var(const char *name, char **env)
{
    char *env_var;
    int n = my_strlen(name);

    for (int i = 0; env != NULL; i++) {
        env_var = env[i];
        if (my_strncmp(name, env_var, n) == 0)
            return (env_var + n + 1);
    }
    return (NULL);
}
