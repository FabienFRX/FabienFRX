/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-calendar-fabien.fraixanet
** File description:
** function_base_error
*/

#include "syn.h"

/* my_putchar_error function: - display 1 character on error output */
static void my_putchar_error(char c)
{
    write(2, &c, 1);
}

/* my_putstr_error function: - display a string on error output */
int my_putstr_error(const char *str)
{
    write(2, str, strlen(str));
    return (84);
}

/* my_put_nbr_error function: display a number on error output */
int my_put_nbr_error(int nb)
{
    if (nb < 0) {
        nb = nb * (-1);
        my_putchar_error('-');
    }
    if (nb > 9) {
        my_put_nbr_error(nb / 10);
    }
    my_putchar_error(nb % 10 + '0');
    return (0);
}
