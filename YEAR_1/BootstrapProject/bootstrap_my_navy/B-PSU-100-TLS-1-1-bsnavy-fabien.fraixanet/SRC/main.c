/*
** EPITECH PROJECT, 2024
** fabien
** File description:
** main
*/

#include "../include/my.h"

int main(int argc, char **argv)
{
    if (argc == 2 && my_strcmp(argv[1], "-h") == 0) {
        disp_help();
        return (0);
    }
    get_pid(argc, argv);
    return (0);
}
