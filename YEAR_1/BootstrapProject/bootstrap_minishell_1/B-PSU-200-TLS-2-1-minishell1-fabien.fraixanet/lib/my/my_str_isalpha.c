/*
** EPITECH PROJECT, 2023
** my_str_isalpha
** File description:
** returns 1 if string only contains alphabetical char and 0 if not
*/

#include <stdbool.h>

bool my_str_isalpha(char const *str)
{
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < 65 || str[i] > 90)
            return (true);
        if (str[i] < 97 || str[i] > 122)
            return (true);
    }
    return (false);
}
