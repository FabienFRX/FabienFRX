/*
** EPITECH PROJECT, 2024
** Position_checker
** File description:
** azerty
*/

#include "../include/my_navy.h"

map_t *m;

int is_right_size(char **pos)
{
    for (int i = 0, j = 1; i != 4; i++, j++) {
        if (pos[i][2] == pos[i][5])
            if ((pos[i][6] - pos[i][3]) != j)
                return 84;
        if (pos[i][2] != pos[i][5])
            if ((pos[i][5] - pos[i][2]) != j)
                return 84;
    }
    return 0;
}

int is_in_map(char **pos)
{
    for (int i = 0; i != 4; i++) {
        if (pos[i][2] < 'A' || pos[i][2] > 'H')
            return 84;
        if (pos[i][5] < 'A' || pos[i][5] > 'H')
            return 84;
    }
    for (int i = 0; i != 4; i++) {
        if (pos[i][3] < '1' || pos[i][3] > '8')
            return 84;
        if (pos[i][6] < '1' || pos[i][6] > '8')
            return 84;
    }
    return 0;
}

int compare_tab_nbr(int *tab)
{
    if (tab[0] != 3 || tab[1] != 4) {
        free (tab);
        return 84;
    }
    if (tab[2] != 5 || tab[3] != 6) {
        free (tab);
        return 84;
    }
    return 0;
}

int is_overlapping(char **pos)
{
    int *tab = malloc(sizeof(int) * 4);

    m->map = populate_empty_map(m->map);
    m->map = fill_map_w_boats(m->map, pos);
    for (int i = 0; m->map[i]; i++)
        for (int j = 0; m->map[i][j]; j++)
            switch (m->map[i][j]) {
            case '2' : tab[0] += 1;
                break;
            case '3' : tab[1] += 1;
                break;
            case '4' : tab[2] += 1;
                break;
            case '5' : tab[3] += 1;
                break;
            }
    if (compare_tab_nbr(tab) == 84) {
        free (m);
        return 84;
    }
    return 0;
}

int position_check(char **pos)
{
    for (int i = 2, j = 0; j != 4; j++, i++) {
        if (my_getnbr(pos[j]) != 1)
            return 84;
    }
    if (is_overlapping(pos) == 84)
        return 84;
    if (is_right_size(pos) == 84)
        return 84;
    if (is_in_map(pos) == 84)
        return 84;
    return 0;
}
