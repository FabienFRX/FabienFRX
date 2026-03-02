/*
** EPITECH PROJECT, 2023
** print_p
** File description:
** function for flag p
*/
#include <stdio.h>
#include "../../include/my.h"
#include "../../include/my_printf.h"

void print_p(void *ptr)
{
    byte_t *ptr_byte;

    ptr_byte = (byte_t *)&ptr;
    for (int i = 7; i >= 0; --i) {
        my_put_nbr_base(*(ptr_byte + i), 16);
    }
}
