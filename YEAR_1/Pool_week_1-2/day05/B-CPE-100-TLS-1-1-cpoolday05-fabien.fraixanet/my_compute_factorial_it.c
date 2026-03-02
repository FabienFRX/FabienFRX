/*
** EPITECH PROJECT, 2023
** my_compute_factorial_it
** File description:
** function qui returns the factorial of the number given dans le parametre
*/

int my_compute_factorial_it(int nb)
{
    int i = 1;
    int res = nb * (nb - i);

    if (nb >= 13) {
        return (0);
    }
    if (nb < 0) {
        return (0);
    }
    if (nb == 0) {
        return (1);
    }
    if (nb == 1) {
        return (1);
    }
    while ((nb - i) > 1 ) {
        i = i + 1;
        res = res * (nb - i);
    }
    return (res);
}
