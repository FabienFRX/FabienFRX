/*
** EPITECH PROJECT, 2024
** SYN_pool
** File description:
** requirement
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_alphnum(char c)
{
    if (c >= 'a' && c <= 'z')
        return (0);
    if (c >= 'A' && c <= 'Z')
        return (1);
    if ((c >= '0' && c <= '9'))
        return (2);
    return (3);
}

// 32 is the difference between 'a' and 'A'
char *my_strcapitalize_synthesis(char *str)
{
    int isCurrentWord = 0;

    if (str == NULL)
        return (NULL);
    if (str[0] >= 'a' && str[0] <= 'z') {
        str[0] -= 32;
        isCurrentWord = 1;
    }
    for (int i = 1; str[i] != '\0'; i++) {
        if (is_alphnum(str[i - 1]) == 3 &&
            is_alphnum(str[i]) == 0 && isCurrentWord == 0) {
            str[i] -= 32;
            isCurrentWord = 1;
        }
        if (is_alphnum(str[i]) == 3 && isCurrentWord == 1) {
            isCurrentWord = 0;
        }
    }
    return (str);
}
