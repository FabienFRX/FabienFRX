/*
** EPITECH PROJECT, 2023
** B-MUL-200-TLS-2-1-myrpg-fabien.fraixanet
** File description:
** test.c
*/

#include "../../include/rpg.h"


//{(sfVector2f){ * 5.0,  * 5.0}, (sfVector2f){ * 5.0, * 5.0}, 0},
walls_t walls_tab[] = {
    {(sfVector2f){30.0 * 5.0, 1080.0 * 5.0}, (sfVector2f){160.0 * 5.0, 0.0}, 0},
    {(sfVector2f){186.0 * 5.0, 365.0 * 5.0}, (sfVector2f){1093.0 * 5.0, 16.0 * 5.0}, 0},
    {(sfVector2f){1085.0 * 5.0, 146.0 * 5.0}, (sfVector2f){191.0 * 5.0, 0.0}, 0},
    {(sfVector2f){440.0 * 5.0, 70.0 * 5.0}, (sfVector2f){644.0 * 5.0, 162.0 * 5.0}, 0},
    {(sfVector2f){378.0 * 5.0, 36.0 * 5.0}, (sfVector2f){707.0 * 5.0, 234.0 * 5.0}, 0},
    {(sfVector2f){247.0 * 5.0, 36.0 * 5.0}, (sfVector2f){834.0 * 5.0, 269.0 * 5.0}, 0},
    {(sfVector2f){122.0 * 5.0, 38.0 * 5.0}, (sfVector2f){962.0 * 5.0, 304.0 * 5.0}, 0},
    {(sfVector2f){54.0 * 5.0, 162.0 * 5.0}, (sfVector2f){1414.0 * 5.0, 0.0 * 5.0}, 0},
    {(sfVector2f){54.0 * 5.0, 162.0 * 5.0}, (sfVector2f){1667.0 * 5.0, 0.0 * 5.0}, 0},
    {(sfVector2f){18.0 * 5.0, 1080.0 * 5.0}, (sfVector2f){1902.0 * 5.0, 0.0}, 0},
    {(sfVector2f){964.0 * 5.0, 18.0 * 5.0}, (sfVector2f){956.0 * 5.0, 1062.0 * 5.0}, 0},
    {(sfVector2f){887.0 * 5.0, 1.0 * 5.0}, (sfVector2f){0.0, 1080.0 * 5.0}, 0},
    {(sfVector2f){125.0 * 5.0, 17.0 * 5.0}, (sfVector2f){516.0 * 5.0, 144.0 * 5.0}, 0},
    {(sfVector2f){648 * 5.0, 1 * 5.0}, (sfVector2f){1272 * 5.0, 0 * 5.0}, 0},
    {(sfVector2f){61 * 5.0, 130 * 5.0}, (sfVector2f){1467 * 5.0, 2 * 5.0}, 0},
    {(sfVector2f){61 * 5.0, 124 * 5.0}, (sfVector2f){1599 * 5.0, 0 * 5.0}, 0},
    {(sfVector2f){110 * 5.0, 180 * 5.0}, (sfVector2f){899 * 5.0, 483 * 5.0}, 0},
    {(sfVector2f){181 * 5.0, 139 * 5.0}, (sfVector2f){1606 * 5.0, 941 * 5.0}, 0},
    {(sfVector2f){130 * 5.0, 340 * 5.0}, (sfVector2f){195 * 5.0, 666 * 5.0}, 0},
    {(sfVector2f){69 * 5.0, 320 * 5.0}, (sfVector2f){322 * 5.0, 695 * 5.0}, 0},
    {(sfVector2f){65 * 5.0, 205 * 5.0}, (sfVector2f){385 * 5.0, 719 * 5.0}, 0},
    {(sfVector2f){59 * 5.0, 8 * 5.0}, (sfVector2f){387 * 5.0, 960 * 5.0}, 0},
    {(sfVector2f){22 * 5.0, 40 * 5.0}, (sfVector2f){243 * 5.0, 1000 * 5.0}, 0},
    {(sfVector2f){12 * 5.0, 52 * 5.0}, (sfVector2f){305 * 5.0, 990 * 5.0}, 0},
    {(sfVector2f){12 * 5.0, 52 * 5.0}, (sfVector2f){193 * 5.0, 984 * 5.0}, 0},
    {(sfVector2f){83 * 5.0, 29 * 5.0}, (sfVector2f){241 * 5.0, 649 * 5.0}, 0},
    {(sfVector2f){15 * 5.0, 24 * 5.0}, (sfVector2f){368 * 5.0, 683 * 5.0}, 0},
    {(sfVector2f){100.0, 100.0}, (sfVector2f){1550.0 * 5.0, 120.0 * 5.0}, 31},
    {(sfVector2f){100.0, 100.0}, (sfVector2f){1050.0 * 5.0, 624.0 * 5.0}, 32},
    {(sfVector2f){100.0, 100.0}, (sfVector2f){1070.0, 5040.0}, 33},
    {(sfVector2f){0, 0}, (sfVector2f){0, 0}, 0}
};
