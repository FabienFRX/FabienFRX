/*
** EPITECH PROJECT, 2023
** display
** File description:
** display -h
*/

#include <stdlib.h>
#include "include/graphic.h"
#include "include/struct.h"
#include "include/my.h"

void display_h(void)
{
    my_printf("Air traffic simulation panel\n\n");
    my_printf("USAGE\n");
    my_printf("  ./my_radar [OPTIONS] path_to_script\n");
    my_printf("  path_to_script    The path to the script file.\n\n");
    my_printf("OPTIONS\n");
    my_printf("  -h\t\t  print the usage and quit.\n\n");
    my_printf("USER INTERACTIONS\n");
    my_printf("  'L\' key\t\tenable/disable hitboxes and areas.\n");
    my_printf("  'S\' key\t\tenable/disable sprites.\n");
}
