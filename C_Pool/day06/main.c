/*
** EPITECH PROJECT, 2023
** main test
** File description:
** test function
*/
#include <string>

int main()
{
    char *str1 = strdup("hello");
    char *str2 = strdup("test");
    my_putstr(my_strcpy(str1, str2));
    my_putstr(my_strncpy(str1, str2));
    my_putstr(my_revstr(str1, str2));
}
