/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-sbmlparser-fabien.fraixanet
** File description:
** display_help
*/

#include "../include/syn.h"

void display_help(void)
{
    my_putstr(" USAGE\n");
    my_putstr("  ./SBMLparser SBMLfile [-i ID [-e]] [-json]\n\n");
    my_putstr("DESCRIPTION\n");
    my_putstr("  SBMLfile\tSBML file\n");
    my_putstr("  -i ID \tid of the compartment,");
    my_putstr("reaction or product to be extracted\n");
    my_putstr("\t\t(ignored if uncorrect)\n");
    my_putstr("  -e \t\tprint the equation if a reaction ID");
    my_putstr("is given as argument\n");
    my_putstr("\t\t(ignored otherwise\n");
    my_putstr("  -json \ttransform the file into a JSON file\n");
}
