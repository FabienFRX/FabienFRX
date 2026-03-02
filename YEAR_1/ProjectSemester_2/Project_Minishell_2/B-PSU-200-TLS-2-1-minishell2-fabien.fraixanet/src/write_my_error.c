/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** write_my_error
*/

#include "my_lib_incl.h"

void write_error_prefix(void)
{
    my_putstr("Error: ", 2);
}

void write_error_message(char *str)
{
    my_putstr(str, 2);
}

void write_error_suffix(void)
{
    my_putstr("\n", 2);
}

void write_my_error(char *str)
{
    write_error_prefix();
    write_error_message(str);
    write_error_suffix();
}
