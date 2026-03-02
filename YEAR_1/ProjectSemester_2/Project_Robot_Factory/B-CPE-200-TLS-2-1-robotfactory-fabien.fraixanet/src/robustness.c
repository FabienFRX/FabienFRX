/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-robotfactory-fabien.fraixanet [WSL: Debian]
** File description:
** robustness
*/

#include "../include/asm.h"

int display_help(void)
{
    my_printf("USAGE\n./asm file_name[.s]\n");
    my_printf("DESCRIPTION\nfile_name in assembly language ");
    my_printf("to be converted into file_name.cor, an executable ");
    my_printf("in the virtual Machine.\n");
    return 0;
}

static int filepath_verify(char *filepath)
{
    int i = 0;

    for (; filepath[i]; i++);
    if (i <= 2 || my_strncmp(filepath + i - 2, ".s", 2) != 0)
        return my_putstr_error("Incorect file extension. \".s\" expected\n");
    return 0;
}

int robustness(int argc, char *argv[])
{
    if (argc != 2)
        return my_putstr_error("Invalid number of arguments\n");
    if (filepath_verify(argv[1]) == 84)
        return 84;
    return 0;
}
