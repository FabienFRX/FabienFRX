/*
** EPITECH PROJECT, 2023
** my_strncmp
** File description:
** compares two string based on their ascii value over a length of n char
*/

#include <stddef.h>

int my_strncmp(const char *s1, const char *s2, size_t n)
{
    while (n > 0) {
        if (*s1 != *s2) {
            return (*s1 - *s2);
        }
        if (*s1 == '\0') {
            break;
        }
        s1++;
        s2++;
        n--;
    }
    return 0;
}
