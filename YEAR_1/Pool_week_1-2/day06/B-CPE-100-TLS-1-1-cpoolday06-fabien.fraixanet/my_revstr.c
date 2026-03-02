/*
** EPITECH PROJECT, 2023
** my_revstr
** File description:
** function that reverse a string
*/

int my_strlen(char const *str)
{
    int i = 0;

    while (str[i] != '\0') {
        i = i + 1;
    }
    return (i);
}

char *my_revstr(char *str)
{
    int first = 0;
    int last = my_strlen(str) - 1;
    char tmp = 0;

    while (first < last) {
        tmp = str[first];
        str[first] = str[last];
        str[last] = tmp;
        first = first + 1;
        last = last - 1;
    }
    return (str);
}
