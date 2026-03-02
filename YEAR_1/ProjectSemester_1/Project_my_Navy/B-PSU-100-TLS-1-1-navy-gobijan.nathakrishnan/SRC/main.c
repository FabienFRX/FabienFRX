/*
** EPITECH PROJECT, 2024
** My_Navy project
** File description:
** PSU Final project
*/

#include "../include/my_navy.h"

/*player_t *p;
map_t *m;

void fill_struct(void)
{
    p->pid = 0;
    p->pid_2 = 0;
    p->check = 0;
    p->attack = 0;
    p->count = 0;
    p->x = 0;
    p->y = 0;
    p->check_eof = 0;
}*/

int main(int argc, char *argv[])
{
    int ret = 0;

    p = malloc(sizeof(player_t));
    m = malloc(sizeof(map_t));
    if (argc == 2 && my_strcmp(argv[1], "-h"))
        return (disp_help());
    if (error_handling(argc, argv) == 84)
        return 84;
    fill_struct();
    if (argc >= 2) {
        if (argc == 2)
            ret = player_uno(m);
        if (argc == 3) 
            if ((ret = player_due(argv, m)) == -1)
                return 0;
    }
    free(p);
    free(m);
    return ret;
}
