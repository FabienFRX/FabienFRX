/*
** EPITECH PROJECT, 2023
** my_putstr
** File description:
** function qui affiche 1 a 1 les charactere d'un string
*/
int my_putstr(char const *str)
{
    int i = 0;

    while (str[i] != '\0') {
        my_putchar(str[i]);
        i = i +1;
    }
    return (0);
}
