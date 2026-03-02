/*
** EPITECH PROJECT, 2024
** B-PSU-200-TLS-2-1-minishell1-fabien.fraixanet
** File description:
** env
*/

#include "../include/my.h"

void my_env(env_t *envp)
{
    for (int i = 0; envp->curr_env[i] != NULL; i++) {
        my_putstr(envp->curr_env[i]);
        my_putchar('\n');
    }
}
