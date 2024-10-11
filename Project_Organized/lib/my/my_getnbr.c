/*
** EPITECH PROJECT, 2023
** my_getnbr
** File description:
** returns a number, sent to the function as a string
*/

#include <stdlib.h>

int my_getnbr(char const *str)
{
    int res = 0;
    int negative = 0;

    if (str == NULL) {
        return (0);
    }
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '-') {
            negative = 1;
            continue;
        }
        res = res * 10 + (str[i] - '0');
    }
    if (negative == 1) {
        res = res * -1;
    }
    return (res);
}
