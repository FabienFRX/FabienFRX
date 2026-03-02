/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Argument parsing functions
*/

#include "server.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

bool parse_args(int ac, char **av, arguments_t *args)
{
    int i = 1;
    memset(args, 0, sizeof(arguments_t));
    while (i < ac) {
        if (strcmp(av[i], "-p") == 0 && i + 1 < ac) {
            args->port = atoi(av[++i]);
        } else if (strcmp(av[i], "-x") == 0 && i + 1 < ac) {
            args->width = atoi(av[++i]);
        } else if (strcmp(av[i], "-y") == 0 && i + 1 < ac) {
            args->height = atoi(av[++i]);
        } else if (strcmp(av[i], "-n") == 0 && i + 1 < ac) {
            i++;
            int team_count = 0;
            args->team_names = malloc(sizeof(char *) * (ac + 1));
            if (!args->team_names)
                return false;
            while (i < ac && av[i][0] != '-') {
                args->team_names[team_count++] = av[i++];
            }
            args->team_names[team_count] = NULL;
            i--;
        } else if (strcmp(av[i], "-c") == 0 && i + 1 < ac) {
            args->clients_nb = atoi(av[++i]);
        } else if (strcmp(av[i], "-f") == 0 && i + 1 < ac) {
            args->freq = atoi(av[++i]);
        } else {
            return false;
        }
        i++;
    }
    return true;
} 