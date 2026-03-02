/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_setenv
*/

#include "my_lib_incl.h"

static char **populate_env(char **cmd, char **env, char **result, char *temp)
{
    int i = 0;

    for (; env[i]; i++) {
        if (my_strcmp(env[i], temp) == 0) {
            free(env[i]);
            break;
        }
        result[i] = env[i];
    }
    result[i] = my_strcat(cmd[1], "=");
    if (cmd[2] != NULL) {
        result[i] = my_strcat(result[i], cmd[2]);
    }
    return result;
}

static char **create_env(char **cmd, char **env)
{
    char **result = NULL;
    char *temp = NULL;

    temp = my_getenv(env, cmd[1]);
    if (temp == NULL) {
        result = my_calloc_ar(my_strlen_array(env) + 1);
    } else {
        result = env;
    }
    result = populate_env(cmd, env, result, temp);
    if (temp == NULL) {
        free(env);
    }
    return result;
}

char **my_setenv(char **cmd, char **env)
{
    if (my_strlen_array(cmd) == 1) {
        my_env(cmd, env);
        return env;
    } else if (my_strlen_array(cmd) > 3) {
        my_putstr("setenv: Too many arguments.\n", 2);
        return env;
    }
    env = create_env(cmd, env);
    return env;
}
