/*
** EPITECH PROJECT, 2023
** my_strncmp
** File description:
** compares two string based on their ascii value over a length of n char
*/

int my_strncmp(char const *s1, char const *s2, int n)
{
    int i = 0;

    while (s1[i] == s2[i] && (s1[i] != '\0' || s2[i] != '\0') && i < n) {
        i = i + 1;
    }
    if (s1[i] < s2[i]) {
        return (-1);
    }
    if (s1[i] > s2[i]) {
        return (1);
    }
    return (0);
}
