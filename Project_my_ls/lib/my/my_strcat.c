/*
** EPITECH PROJECT, 2023
** my_strcat
** File description:
** concatenates two string
*/

#include "../../include/my.h"

char *my_strcat(char *dest, char const *src)
{
    my_strcpy(dest + my_strlen(dest), src);
    return (dest);
}
