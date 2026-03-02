/*
** EPITECH PROJECT, 2023
** display_h
** File description:
** function for display
*/

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "include/my.h"
#include <string.h>

void display_h(void)
{
    printf("USAGE\n");
    printf("\t./103cipher message key flag\n\n");
    printf("DESCRIPTION\n");
    printf("\tmessage\t\ta message, made of ASCII characters\n");
    printf("\tkey\t\tthe encryption key, made of ASCII characters\n");
    printf("\tflag\t\t0 for the message to be encrypted, 1 to be decrypted\n");
}
