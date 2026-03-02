/*
** EPITECH PROJECT, 2023
** my_compute_factorial_rec
** File description:
** function recursive qui return factorial of the number in parameter
*/


int my_compute_factorial_rec(int nb)
{
    int res = 1;

    if (nb >= 13) {
        return (0);
    }
    if (nb < 0) {
        return (0);
    }
    if (nb == 0) {
        return (1);
    }
    if ((nb - 1) >= 1) {
        res = my_compute_factorial_rec(nb - 1);
        res = res * nb;
    }
    return (res);
}
