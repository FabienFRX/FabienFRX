/*
** EPITECH PROJECT, 2024
** Player uno & Player due
** File description:
** azerty
*/

#include "../include/my_navy.h"

player_t *p;

int hit_counter(char **map)
{
    int hitCount = 0;

    for (int row = 0; row < 10; ++row)
        for (int col = 0; col < 17; ++col)
            if (map[row][col] == 'x')
                ++hitCount;
    return hitCount;
}

int player_uno(map_t *m)
{
    p->turn = 1;
    p->pid = getpid();
    my_printf("my_pid: ");
    my_printf("%d", p->pid);
    my_printf("\nwaiting for enemy connection...\n");
    connection();
    my_printf("\nenemy connected\n\n");
    m->map_e = populate_empty_map(m->map_e);
    display_map(m->map, m->map_e);
    while (game_one(m) != 1);
    if (p->check_eof == 1)
        return 0;
    display_map(m->map, m->map_e);
    if (hit_counter(m->map_e)) {
        my_printf("I won\n");
        return 0;
    } else {
        my_printf("Enemy won\n");
        return 1;
    }
}

static int fine_player_due(map_t *m)
{
    display_map(m->map, m->map_e);
    if (hit_counter(m->map_e) == 14) {
        my_printf("I won\n");
        return 0;
    } else {
        my_printf("Enemy won\n");
        return 1;
    }
}

int player_due(char *argv[], map_t *m)
{
    char **pos = openfile(argv[2]);

    p->turn = 0;
    p->pid = getpid();
    p->pid_2 = my_getnbr(argv[1]);
    my_printf("my_pid: %d", p->pid);
    if (kill(p->pid_2, SIGUSR1) == -1)
        return -1;
    my_printf("\nsuccessfully connected\n\n");
    m->map = populate_empty_map(m->map);
    m->map = fill_map_w_boats(m->map, pos);
    m->map_e = populate_empty_map(m->map_e);
    display_map(m->map, m->map_e);
    while (game_two(m) != 0);
    if (p->check_eof == 1)
        return 0;
    return fine_player_due(m);
}