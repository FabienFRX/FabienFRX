/*
** EPITECH PROJECT, 2023
** my_strcmp
** File description:
** compares two strings based on their ascii value
*/
#include "../../include/my.h"

int my_strcmp(char const *s1, char const *s2)
{
    int i = 0;

    if (s1 == NULL || s2 == NULL)
        return (-1);
    if (s1[i] < s2[i]) {
        return (-1);
    }
    if (s1[i] > s2[i]) {
        return (1);
    }
    for (int i = 0; s1[i] != '\0'; i++) {
        if (s1[i] != s2[i]) {
            return (1);
        }
    }
    return (0);
}
