/*
** EPITECH PROJECT, 2023
** my_revstr
** File description:
** returns the reversed version of a string in parameters
*/

int my_str_len(char const *str)
{
    int i = 0;

    while (str[i] != '\0') {
        i += 1;
    }
    return (i);
}

char *my_revstr(char *str)
{
    int x = 0;
    int y = my_str_len(str) - 1;
    char temp;

    while (x < y) {
        temp = str[x];
        str[x] = str[y];
        str[y] = temp;
        x += 1;
        y -= 1;
    }
    return (str);
}
