/*
** EPITECH PROJECT, 2023
** my_compute_power_rec
** File description:
** returns the first argument raised to the
** power of the second one, recursively
*/

int my_compute_power_rec(int nb, int p)
{
    if (p < 0) {
        return (0);
    }
    if (p == 0) {
        return (1);
    }
    if (p > 36000 || nb > 2147483647) {
        return 0;
    }
    return (my_compute_power_rec(nb, p - 1) * nb);
}
