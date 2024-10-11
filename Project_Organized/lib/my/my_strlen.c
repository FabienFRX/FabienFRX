/*
** EPITECH PROJECT, 2023
** my_strlen
** File description:
** counts and returns the number of char in a string passed in parameters
*/

int my_strlen(char const *str)
{
    int i = 0;

    while (str[i] != '\0') {
        i = i + 1;
    }
    return (i);
}
