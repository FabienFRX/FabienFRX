/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-robotfactory-fabien.fraixanet [WSL: Debian]
** File description:
** print_tab
*/

#include "../../include/my.h"
#include <unistd.h>
#include <stdlib.h>

void print_tab(char **tab)
{
    for (int i = 0; tab[i] != NULL; i++) {
        write(1, tab[i], my_strlen(tab[i]));
        write(1, "\n", 1);
    }
}
