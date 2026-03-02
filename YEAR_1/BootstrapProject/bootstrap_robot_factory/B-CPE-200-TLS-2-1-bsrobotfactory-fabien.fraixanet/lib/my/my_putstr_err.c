/*
** EPITECH PROJECT, 2024
** B-PSU-200-TLS-2-1-minishell1-fabien.fraixanet
** File description:
** my_putstr on error output
*/

#include <unistd.h>
#include "../../include/my.h"

int my_putstr_err(char *str)
{
    int i = 0;

    while (str[i] != '\0')
        i++;
    write(2, str, i);
    return (i);
}
