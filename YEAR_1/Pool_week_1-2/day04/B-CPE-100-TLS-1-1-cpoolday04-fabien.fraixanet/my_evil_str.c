/*
** EPITECH PROJECT, 2023
** my_evil_str
** File description:
** swap each char 2 by 2
*/

char *my_evil_str(char *str)
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
