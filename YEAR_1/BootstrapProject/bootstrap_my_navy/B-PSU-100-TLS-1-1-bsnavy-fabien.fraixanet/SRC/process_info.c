/*
** EPITECH PROJECT, 2024
** fabien
** File description:
** process_info
*/

#include "../include/my.h"

int get_pid(int argc, char *argv[])
{
    my_printf("my_pid: %d\n", getpid());
    return(0);
}
