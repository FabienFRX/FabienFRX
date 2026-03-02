/*
** EPITECH PROJECT, 2024
** fabien
** File description:
** disp_help
*/

#include "../include/my.h"

void disp_help(void)
{
    my_printf("USAGE\n");
    my_printf("\t./navy [first_player_pid] navy_positions\n");
    my_printf("DESCRIPTION\n");
    my_printf("\tfirst_player_pid: only for the 2nd player. ");
    my_printf("pid of the first player.\n");
    my_printf("\tnavy_positions: file representing");
    my_printf("the positions of the ships.\n");
}
