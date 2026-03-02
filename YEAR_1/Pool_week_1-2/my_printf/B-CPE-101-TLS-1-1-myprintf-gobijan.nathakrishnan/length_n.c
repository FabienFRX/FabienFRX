/*
** EPITECH PROJECT, 2023
** size_count
** File description:
** return the lenght of the string
*/

int length_n(const char *str)
{
    int count = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ') {
            count++;
        }
    }
    my_put_nbr_base(count, 10);
    return count;
}
