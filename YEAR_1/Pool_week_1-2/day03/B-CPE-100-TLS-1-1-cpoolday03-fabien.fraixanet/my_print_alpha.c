/*
** EPITECH PROJECT, 2023
** my_print_alpha_c
** File description:
** my alpha, in right order
*/

int my_print_alpha(void)

{
    int c = 97;

    while ( c <= 122 ) {
        my_putchar(c);
        c = c + 1;
    }
    return (0);
}
