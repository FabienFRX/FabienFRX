/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-palindrome-fabien.fraixanet
** File description:
** requirement
*/

#include <stdio.h>

int my_factrec_synthesis(int nb)
{
    if (nb < 0 || nb > 12)
        return (0);
    if (nb == 0)
        return (1);
    return (nb * my_factrec_synthesis(nb - 1));
}

int my_squareroot_synthesis(int nb)
{
    if (nb < 0)
        return (-1);
    if (nb == 0)
        return (0);
    if (nb == 1)
        return (1);
    for (int i = 1; (i * i) <= nb; i++) {
        if ((i * i) == nb)
            return (i);
    }
    return (-1);
}
