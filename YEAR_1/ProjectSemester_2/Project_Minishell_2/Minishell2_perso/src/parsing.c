/*
** EPITECH PROJECT, 2024
** B-PSU-200-TLS-2-1-minishell1-fabien.fraixanet
** File description:
** parsing
*/

#include "../include/my.h"

void parse_env(char **argv, char **env)
{
    char **temp;

    if (my_strcmp(argv[0], "./mysh") == 0) {
        execve(argv[0], argv, env);
    }
    for (int i = 0; env[i] != NULL; i++) {
        if (my_strcmp_to_char(env[i], "PATH", '=') == true)
            temp = my_str_to_word_array(env[i], ":=");
    }
    for (int j = 1; temp[j] != NULL; j++) {
        if (access(my_strcat(my_strcat(temp[j], "/"), argv[0]), X_OK) == 0) {
            execve(my_strcat(my_strcat(temp[j], "/"),
                argv[0]), argv, env);
        }
    }
}
