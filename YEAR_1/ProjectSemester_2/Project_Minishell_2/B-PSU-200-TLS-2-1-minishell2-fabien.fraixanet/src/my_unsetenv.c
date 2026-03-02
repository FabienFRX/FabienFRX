/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_unsetenv
*/

#include "my_lib_incl.h"

void shift_env_lines(char **env, int start)
{
    int i = start;

    while (env[i + 1] != NULL) {
        env[i] = env[i + 1];
        i++;
    }
    env[i] = NULL;
}

int is_matching_env(char *env_entry, char *value)
{
    return my_strncmp(env_entry, value, my_strlen(value)) == 0;
}

static char **delete_line_env(char **env, char *variable)
{
    char *value = my_strcat(variable, "=");
    int i = 0;
    int env_len = my_strlen_array(env);

    while (i < env_len && !is_matching_env(env[i], value)) {
        i++;
    }
    if (i < env_len) {
        shift_env_lines(env, i);
    }
    free(value);
    return env;
}

char **my_unsetenv(char **cmd, char **env)
{
    if (my_strlen_array(cmd) == 1) {
        my_putstr("unsetenv: Too few arguments.\n", 2);
        return env;
    }
    if (env == NULL)
        return NULL;
    for (int i = 1; cmd[i]; i++) {
        env = delete_line_env(env, cmd[i]);
    }
    return env;
}
