/*
** EPITECH PROJECT, 2023
** my_strncpy
** File description:
** my_strncpy pour lib
*/

char *my_strncpy(char *dest, char const *src, int n)
{
    int j = 0;

    while (src[j] != '\0' && j < n) {
        dest[j] = src[j];
        j = j + 1;
    }
    if (src[1] == '\0') {
    dest[j] = '\0';
    }
    return (dest);
}
