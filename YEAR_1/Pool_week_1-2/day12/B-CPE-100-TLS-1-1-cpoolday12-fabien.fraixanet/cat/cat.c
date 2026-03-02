/*
** EPITECH PROJECT, 2023
** cat
** File description:
** cat program
*/

#include "../include/my.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int cat_stdin(void)
{
    int size = -1;
    char buffer[32000];

    while (size != 0) {
        size = read(0, buffer, 32000);
        write(1, buffer, size);
    }
    my_putstr("coucou");
    return (0);
}

int cat_file(int ac, char **av)
{
    int fd = -1;
    int size = 0;
    int tmp = 1;
    char buffer[32000];

    while (tmp != ac) {
        size = -1;
        fd = open(av[tmp], O_RDONLY);
        if (fd == -1) {
            return (84);
        }
        while (size != 0) {
            size = read(fd, buffer, 32000);
            write(1, buffer, size);
        }
        tmp = tmp + 1;
        close(fd);
    }
    return (0);
}

int main(int ac, char **av)
{
    if (ac == 1) {
        return (cat_stdin());
    } else {
        return (cat_file(ac, av));
    }
}
