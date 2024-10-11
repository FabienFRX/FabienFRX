/*
** EPITECH PROJECT, 2023
** my_compute_power_rec
** File description:
** returns the first argument raised to the power of the second one
*/

int my_compute_power_rec(int nb, int p)
{
    int res = 1;

    if (p == 0) {
        return (1);
    }
    if (p < 0) {
        return (0);
    }
    res = my_compute_power_rec(nb, p - 1);
    res = nb * res;
    return (res);
}
