/*
** EPITECH PROJECT, 2024
** B-PSU-200-TLS-2-1-minishell1-fabien.fraixanet
** File description:
** my_putchar on error output
*/

#include <unistd.h>
#include "../../include/my.h"

void my_putchar_err(char c)
{
    write(2, &c, c);
    return;
}
