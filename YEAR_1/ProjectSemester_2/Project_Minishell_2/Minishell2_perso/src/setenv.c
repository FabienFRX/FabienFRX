/*
** EPITECH PROJECT, 2024
** B-PSU-200-TLS-2-1-minishell1-fabien.fraixanet
** File description:
** setenv
*/

#include "../include/my.h"

bool syntax_setenv(char **argv)
{
    if (my_arrlen(argv) < 3) {
        my_putstr("setenv: Too few arguments\n");
        return (true);
    }
    if (my_arrlen(argv) > 3) {
        my_putstr("setenv: Too many arguments\n");
        return (true);
    }
    if (argv[1][0] < 'A' || argv[1][0] > 'Z' &&
        argv[1][0] < 'a' || argv[1][0] > 'z') {
        my_putstr("setenv: Variables must begin with a letter\n");
        return (true);
    }
    return (false);
}

bool my_strcmp_to_char(char *s1, char *s2, char c)
{
    if (s1 == NULL || s2 == NULL)
        return (false);
    for (int i = 0; s1[i] != c; i++) {
        if (s1[i] != s2[i])
            return (false);
    }
    return (true);
}

int in_env(char **temp, char *argv, int arg)
{
    for (int i = 0; temp[i] != NULL; i++) {
        if (my_strcmp_to_char(temp[i], argv, '=') == true)
            return (i);
    }
    return (arg);
}

void my_setenv(char **argv, char **env)
{
    int len_env = my_arrlen(env);
    int arg = len_env;
    char **temp = malloc(sizeof(char *) * (my_arrlen(env) + 1));
    char *tmpenv;

    if (syntax_setenv(argv) == true)
        return;
    tmpenv = my_strcat(argv[1], "=");
    tmpenv = my_strcat(tmpenv, argv[2]);
    temp = env;
    arg = in_env(temp, argv[1], arg);
    temp[arg] = tmpenv;
    temp[len_env + 1] = NULL;
    env = temp;
}
