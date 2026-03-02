/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04am-fabien.fraixanet
** File description:
** mul_div
*/

#include <stdio.h>

void mul_div_long(int a, int b, int *mul, int *div)
{
    if (mul)
        *mul = a * b;
    if (div)
        *div = (b == 0) ? 42 : a / b;
}

void mul_div_short(int *a, int *b)
{
    int temp = *a;

    if (a && b) {
        *a = temp * (*b);
        *b = (*b != 0) ? temp / (*b) : 42;
    }
}
