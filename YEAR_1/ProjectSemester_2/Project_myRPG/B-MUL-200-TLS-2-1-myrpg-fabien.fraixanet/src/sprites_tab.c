/*
** EPITECH PROJECT, 2023
** B-MUL-200-TLS-2-1-myrpg-fabien.fraixanet
** File description:
** sprites_tab.c
*/

#include "../include/rpg.h"

sprites_t sprite_tab[] = {
    {"./src_texture/inventaire.png", {6.0 * 1.77777, 6.0},
        {0, 0}, {10000.0, 10000.0}, "inventaire"},
    {"./src_texture/yasuo.png", {1, 1}, {0, 0}, {0, 0}, "enemies"},
    {"./src_texture/pnj/goldorack.png", {1, 0.5}, {0, 0}, (sfVector2f){1540.0 * 8.9, 100 * 5.0}, "goldorack"},
    {"./src_texture/pnj/batman.png", {4, 2}, {0, 0}, (sfVector2f){1830.0, 5000.0}, "batman"},
    {"./src_texture/pnj/CaptainHarlock.png", {0.55, 0.35}, {0, 0}, (sfVector2f){1850.0 * 5.0, 620.0 * 5.0}, "harlock"},
    {NULL, {1, 1}, {0, 0}, {0, 0}, NULL}
};
