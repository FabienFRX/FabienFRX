/*
** EPITECH PROJECT, 2023
** music
** File description:
** music_my_hunter
*/

#include <stdio.h>
#include <stdlib.h>
#include "include/graphic.h"

int start_music(sfMusic *music)
{
    music = sfMusic_createFromFile("./my_hunter_song.wav");
    if (music == NULL) {
        return (84);
    }
    sfMusic_play(music);
    return (0);
}
