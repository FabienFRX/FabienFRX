/*
** EPITECH PROJECT, 2024
** B-MUL-100-TLS-1-1-myradar-fabien.fraixanet
** File description:
** init
*/

#include "struct.h"
#include "graphic.h"

#ifndef INIT_H_
    #define INIT_H_
void init_window(GLOB_T *all);
static int init_map(GLOB_T *all);
//tower
int init_tower(GLOB_T *all);
int tower_2nd(GLOB_T *all);
int init_sprite(GLOB_T *all);
int init(GLOB_T *all);
//plane
int init_plane(GLOB_T *all);
int plane_2nd(GLOB_T *all);
int plane_3rd(GLOB_T *all);
int init_sprite_pla(GLOB_T *all);
int inity(GLOB_T *all);

#endif /* !INIT_H_ */
