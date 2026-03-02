/*
** EPITECH PROJECT, 2023
** my_compute_square_root
** File description:
** returns the square root of a number
** passed in parameters
*/

int my_compute_square_root(int nb)
{
    int i = 1;

    if (nb == 0 || nb > 2147483647) {
        return 0;
    }
    if (nb < 0) {
        return 0;
    }
    while ((i * i) != nb) {
        if ((i * i) > nb) {
            return 0;
        }
        i += 1;
    }
    return i;
}
