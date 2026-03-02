/*
** EPITECH PROJECT, 2023
** my_print_revalpha
** File description:
** alpha in wrong order.
*/

int my_print_revalpha(void)

{
    int c = 122;

    while ( c >= 97 ) {
        my_putchar(c);
        c = c - 1;
    }
    return (0);
}
