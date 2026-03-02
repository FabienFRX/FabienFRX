/*
** EPITECH PROJECT, 2023
** my_isneg_c
** File description:
** function for negative number =N and positive number =P.
*/

int my_isneg(int n)

{
    if ( n < 0 ) {
        my_putchar('N');
    } else {
        my_putchar('P');
    }
    return (0);
}
