/*
** EPITECH PROJECT, 2024
** Error management
** File description:
** azerty
*/

#include "../include/my_navy.h"

int error_management_player1(char *filepath)
{
    char **pos = openfile(filepath);

    if (pos == NULL)
        return 84;
    if (position_check(pos) == 84){
        free(pos);
        return 84;
    }
    free(pos);
    return 0;
}

int error_management_player2(char *filepath)
{
    char **pos = openfile(filepath);

    if (pos == NULL)
        return 84;
    if (position_check(pos) == 84) {
        free(pos);
        return 84;
    }
    free(pos);
    return 0;
}

int error_handling(int argc, char *argv[])
{
    if (argc < 2 || argc > 3) {
        return 84;
    }
    if (argc == 2) {
        if (error_management_player1(argv[1]) == 84) {
            return 84;
        }
    }
    if (argc == 3) {
        if (error_management_player2(argv[2]) == 84) {
            return 84;
        }
    }
    return 0;
}
