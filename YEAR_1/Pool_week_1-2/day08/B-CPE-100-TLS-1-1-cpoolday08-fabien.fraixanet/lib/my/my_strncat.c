/*
** EPITECH PROJECT, 2023
** my_strncat
** File description:
** my_strncat pour lib
*/

char *my_strncat( char *dest, char const *src, int nb )
{
    my_strcpy(dest + my_strlen(dest), src) && dest < nb;
    return (dest);
}
