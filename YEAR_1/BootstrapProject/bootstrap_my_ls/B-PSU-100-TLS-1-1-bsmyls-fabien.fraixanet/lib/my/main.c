/*
** EPITECH PROJECT, 2023
** main
** File description:
** main test
*/
#include <stdio.h>

int main(void)
{
    char *str = "jhon";
    float f = 12.45;
    int loc = 16;
    int *ptr = &loc;
    unsigned int nb = -12;
    double en = 11357.213548;
    double ne = -112354.458796;

    printf("hello, %d, %E, %b, %X, %e, %s, %f, %p, %x, %u\n", 47, ne, -123, -365, en, str, f, ptr, -134, nb);
    my_printf("hello %d, %E, %b, %X, %e, %s, %f, %p, %x, %u\n", 47, ne, -123, -365, en, str, f, ptr, -134, nb);
    return (0);
}
