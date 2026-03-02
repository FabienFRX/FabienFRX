/*
** EPITECH PROJECT, 2025
** MinilibC
** File description:
** my_putchar
*/

#include <unistd.h>

void my_putchar(char c)
{
    write (1, &c, 1);
}
