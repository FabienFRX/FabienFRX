/*
** EPITECH PROJECT, 2023
** my_putchar_c
** File description:
** my putchar, pour pouvoir display.
*/

#include <unistd.h>
int my_putchar(char c)

{
    write( 1, &c, 1 );
    return (0);
}
