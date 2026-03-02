/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-amazed-fabien.fraixanet [WSL: Debian]
** File description:
** main
*/

#include "../include/amazed.h"

void init_tunnel(tunnel_t *tunnels)
{
    tunnels->room_id1 = 0;
    tunnels->room_id2 = 0;
}

void init_room_info(room_info_t *rooms)
{
    room_info_t *current_room = (room_info_t *)malloc(sizeof(room_info_t));

    rooms->id = 0;
    rooms->x = 0;
    rooms->y = 0;
    rooms->name = NULL;
    rooms->is_start = 0;
    rooms->is_end = 0;
}

int main(int argc, char **argv)
{
    room_info_t *current_room = (room_info_t *)malloc(sizeof(room_info_t));
    tunnel_t *tunnels = (tunnel_t *)malloc(sizeof(tunnel_t));

    (void)argv;
    if (argc != 1)
        return 84;
    init_room_info(current_room);
    init_tunnel(tunnels);
    parse_labyrinth();
    return 0;
}
