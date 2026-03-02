/*
** EPITECH PROJECT, 2023
** digits
** File description:
** digits in order acsending
*/

int my_print_digits(void)

{
    int c = 48;

    while ( c <= 57 ) {
        my_putchar(c);
        c = c + 1;
    }
    return (0);
}
