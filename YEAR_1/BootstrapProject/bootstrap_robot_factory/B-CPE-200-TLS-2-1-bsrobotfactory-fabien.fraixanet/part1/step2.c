/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-bsrobotfactory-fabien.fraixanet
** File description:
** step2
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/my.h"

int main(void)
{
    FILE *fp = fopen("./number-as-text.yolo", "w+");

    fprintf(fp, "12345678\n");
    fclose(fp);
    return (0);
}
