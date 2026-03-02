/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_putstr
*/

#include "my_lib_incl.h"

void write_string_to_fd(char *str, int fd)
{
    write(fd, str, my_strlen(str));
}

void my_putstr(char *str, int fd)
{
    if (str == NULL)
        return;
    write_string_to_fd(str, fd);
}
