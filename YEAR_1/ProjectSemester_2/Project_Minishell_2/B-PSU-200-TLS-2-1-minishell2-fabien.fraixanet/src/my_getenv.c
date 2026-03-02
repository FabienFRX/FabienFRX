/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_getenv
*/

#include "my_lib_incl.h"

char *find_variable_in_env(char **env, char *temp)
{
    int i = 0;

    while (env[i] != NULL) {
        if (my_strncmp(env[i], temp, my_strlen(temp)) == 0)
            return env[i];
        i++;
    }
    return NULL;
}

char *my_getenv(char **env, char *variable)
{
    char *temp = NULL;
    char *result = NULL;

    if (env == NULL || variable == NULL)
        return NULL;
    temp = my_strcat(variable, "=");
    if (temp == NULL)
        return NULL;
    result = find_variable_in_env(env, temp);
    free(temp);
    return result;
}
