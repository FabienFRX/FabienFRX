/*
** EPITECH PROJECT, 2023
** fabien
** File description:
** disp_help
*/

#include "../include/my.h"

void disp_help(void)
{
    my_printf("USAGE\n");
    my_printf("\t./my_sokoban map\n");
    my_printf("DESCRIPTION\n\tmap\tfile representing the warehouse map,");
    my_printf("containing'#' for walls, 'P' for the player,");
    my_printf("'X' for boxes and 'O' for storage locations.\n");
}
