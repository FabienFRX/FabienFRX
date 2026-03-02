/*
** EPITECH PROJECT, 2023
** my_strlen
** File description:
** my_strlen pour lib
*/

int my_strlen(char const *str)
{
    int i = 0;

    while (str[i] != '\0') {
        i = i + 1;
    }
    return (i);
}
