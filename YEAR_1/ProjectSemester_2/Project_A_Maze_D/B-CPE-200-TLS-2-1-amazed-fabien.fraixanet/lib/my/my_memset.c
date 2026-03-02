/*
** EPITECH PROJECT, 2024
** my_memset
** File description:
** initialization memory
*/
#include <stddef.h>

void my_memset(void *ptr, int value, size_t num_bytes)
{
    unsigned char *p = (unsigned char *)ptr;
    size_t i;

    for (i = 0; i < num_bytes; i++) {
        p[i] = (unsigned char)value;
    }
}
