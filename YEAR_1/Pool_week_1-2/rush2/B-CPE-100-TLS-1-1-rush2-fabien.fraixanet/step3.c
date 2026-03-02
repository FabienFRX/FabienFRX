/*
** EPITECH PROJECT, 2023
** rush2 step3
** File description:
** adding frequencies of each letter
*/

#include <stdio.h>

void my_putchar(char c)
{
    write(1, &c, 1);
}

static void my_put_nbr(int nb)
{
    if (nb > 9) {
        my_put_nbr(nb / 10);
    } else if (nb < 0) {
        nb *= (-1);
        write(1, '-', 1);
        my_put_nbr(nb / 10);
    }
    my_putchar(nb % 10 + '0');
}

static int cmp_letters(char c1, char c2)
{
    char c1_alt = 0;

    if (c1 >= 'A' && c1 <= 'Z') {
        c1_alt = c1 + 32;
    }
    if (c1 >= 'a' && c1 <= 'z') {
        c1_alt = c1 - 32;
    }
    if (c1 == c2 || c2 == c1_alt) {
        return (1);
    }
    return (0);
}

static int condition(char *argv, char *str, int res, int i)
{
    if (cmp_letters(argv[0], str[i]) == 1) {
        res += 1;
    }
    return res;
}

int my_strlen(char const *str)
{
    int i = 0;

    while (str[i] != '\0') {
        i = i + 1;
    }
    return (i);
}

int my_put_percent(int percent)
{
    int i = 0;

    if (i == 2) {
        my_putchar(',');
        if (percent > 9) {
            i = i - 1;
            my_put_nbr(percent / 10);
        } else if (percent < 0) {
            percent *= (-1);
            write(1, '-', 1);
            i = i - 1;
            my_put_nbr(percent / 10);
        }
        my_putchar(percent % 10 + '0');
    }
}

static int frequencies(int res, char **argv)
{
    char *str = argv[1];
    int length = my_strlen(str);
    int percent;

    percent = (length * 1000 / 100) * res;
    return (my_put_percent(percent));
}

static void my_print(char **argv, int res, int j)
{
    my_putchar(argv[j][0]);
    my_putchar(':');
    my_put_nbr(res);
    my_putchar(' ');
    my_putchar('(');
    my_put_nbr(frequencies(res, argv));
    my_putchar('%');
    my_putchar(')');
    my_putchar('\n');
}

int step2(int argc, char **argv)
{
    int i;
    int res = 0;
    int j = 2;
    char *str = argv[1];

    while (j < argc) {
        i = 0;
        while (str[i] != 0) {
            res = condition(argv[j], str, res, i);
            i++;
        }
        my_print(argv, res, j);
        j += 1;
        res = 0;
    }
    return 0;
}

int main(int argc, char **argv)
{
    step2(argc, argv);
    return 0;
}
