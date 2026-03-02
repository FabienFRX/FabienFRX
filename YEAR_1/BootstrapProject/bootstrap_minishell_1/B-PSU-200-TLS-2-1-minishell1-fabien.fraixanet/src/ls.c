/*
** EPITECH PROJECT, 2024
** B-PSU-200-TLS-2-1-minishell1-fabien.fraixanet
** File description:
** ls
*/

#include "../include/my.h"

void my_ls(char **argv, char **env)
{
    pid_t p;
    int status;

    p = fork();
    if (p == 0) {
        execve("/bin/ls", argv, env);
        my_putchar('\n');
        exit(0);
    }
    waitpid(p, &status, 0);
}
