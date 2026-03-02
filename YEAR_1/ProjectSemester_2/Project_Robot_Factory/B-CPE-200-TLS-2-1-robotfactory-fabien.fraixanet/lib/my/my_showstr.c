/*
** EPITECH PROJECT, 2023
** my_showstr
** File description:
** print a string and return 0. if the string contains non-printable char
** they must be printed hexadecimally with a backslash before the given value
*/
#include "../../include/my.h"
#include <unistd.h>

int my_showstr(char const *str)
{
    int i = 0;

    while (str[i] != '\0') {
        if (str[i] < 32) {
            my_putchar('\\');
            my_put_nbr_hexa(str[i]);
        } else {
            my_putchar(str[i]);
        }
        i = i + 1;
    }
    return (0);
}
