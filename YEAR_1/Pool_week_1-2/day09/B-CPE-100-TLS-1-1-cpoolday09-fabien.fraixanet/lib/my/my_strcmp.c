/*
** EPITECH PROJECT, 2023
** my_strcmp
** File description:
** my_strcmp pour lib
*/

int my_strcmp(char const *s1, char const *s2)
{
    int i = 0;

    while (s1[i] == s2[i] && (s1 != '\0' || s2 != '\0')) {
        i = i + 1;
    }
    return (0);
}
