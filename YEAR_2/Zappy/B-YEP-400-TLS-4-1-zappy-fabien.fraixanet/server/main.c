/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Main function
*/

#include "server.h"

int main(int ac, char **av)
{
    arguments_t args;
    server_t server;
    if (!parse_args(ac, av, &args))
        return 84;
    init_server(&server, &args);
    init_server_components(&server);
    run_server(&server);
    return 0;
} 