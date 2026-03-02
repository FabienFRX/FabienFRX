/*
** EPITECH PROJECT, 2023
** my_swap
** File description:
** echange de deux entiers
*/
#include <unistd.h>
void my_swap( int *a, int *b )
{
    int c = 0;

    c = *a;
    *a = *b;
    *b = c;
    return;
}
