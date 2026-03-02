/*
** EPITECH PROJECT, 2023
** my_isneg
** File description:
** Displays N if negative, or P if positive or null
*/

int    my_isneg(int n)
{
    if (n < 0) {
        my_putchar('N');
        my_putchar(10);
        return (0);
    }
    my_putchar('P');
    my_putchar(10);
    return (0);
}
