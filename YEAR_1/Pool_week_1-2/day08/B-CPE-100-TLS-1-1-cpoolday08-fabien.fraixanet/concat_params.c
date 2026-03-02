/*
** EPITECH PROJECT, 2023
** concat_params
** File description:
** function that turns the command-line given arguments into a single string. Arguments are to be separated by ‘\n’
*/
#include <unistd.h>

void my_putchar(char c)
{
    write(1, &c, 1);
}

void my_putdigit(int n)
{
    my_putchar('0' + n);
}

int my_putstr(char const *str)
{
    int i = 0;

    while (str[i] != '\0') {
        my_putchar(str[i]);
        i = i +1;
    }
    return (0);
}

char *my_strcpy(char *dest, char const *src)
{
    int i = 0;

    while (src[i] != '\0') {
        dest[i] = src[i];
        i = i + 1;
    }
    dest[i] = '\0';
    return (dest);
}

char *concat_params(int argc, char **argv)
{
    char *str;
    char *src = "hello world";
    char dest[60] = {0};

    my_strcpy(dest, src);
    my_putstr(dest);
}

int main(int ac, char **av)
{
    //char *str = "hello world";
    //char c = str[0];

    //my_putdigit(ac);
    //my_putchar('\n');
    //my_putstr(av[1]);
    //my_putchar('\n');

    concat_params(ac , av);
    return (0);
}
