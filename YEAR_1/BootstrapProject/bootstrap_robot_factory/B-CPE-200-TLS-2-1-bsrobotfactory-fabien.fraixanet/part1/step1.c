/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-bsrobotfactory-fabien.fraixanet
** File description:
** step1
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/my.h"

int main(void)
{
    FILE *fp = fopen("./some-text.yolo.", "w+");

    fprintf(fp, "Hello bambino\n");
    fclose(fp);
    return (0);
}
