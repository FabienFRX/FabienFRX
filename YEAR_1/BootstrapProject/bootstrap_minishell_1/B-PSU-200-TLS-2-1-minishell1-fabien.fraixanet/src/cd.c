/*
** EPITECH PROJECT, 2024
** B-PSU-200-TLS-2-1-minishell1-fabien.fraixanet
** File description:
** cd
*/

#include "../include/my.h"

void my_cd(char **argv, char **env)
{
    if (argv[1] == NULL || my_strcmp(argv[1], "~") == 0) {
        chdir("/home/fabien/");
    } else {
        chdir(argv[1]);
    }
    return;
}
