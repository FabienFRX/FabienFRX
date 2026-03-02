/*
** EPITECH PROJECT, 2023
** put_nbr
** File description:
** put_nbr pour lib
*/

int my_put_nbr(int nb)
{
    if (nb > 9) {
        my_put_nbr(nb / 10);
    } else if (nb < 0) {
        nb *= (-1);
        write(1, '-', 1);
        my_put_nbr(nb / 10);
    }
    my_putchar(nb % 10 + '0');
    return (0);
}
