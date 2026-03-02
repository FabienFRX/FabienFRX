/*
** EPITECH PROJECT, 2023
** graphic.h
** File description:
** graphic include
*/

#ifndef GRAPHIC_H
    #define GRAPHIC_H
    #include <SFML/Window.h>
    #include <SFML/Graphics.h>
    #include <SFML/Audio.h>
    #include <SFML/Graphics/Rect.h>
    #include <SFML/Graphics/RenderWindow.h>
    #include <SFML/Graphics/Texture.h>
    #include <SFML/Graphics/Sprite.h>
    #include "struct.h"

static const unsigned int WIDTH = 1200;
static const unsigned int HEIGHT = 675;
static const unsigned int DUCK_X = 0;
static const unsigned int DUCK_Y = 0;
static const unsigned int SCALE_X = 1;
static const unsigned int SCALE_Y = 1;

int init(GLOB_T *all);
int start_music(sfMusic *music);
void game(GLOB_T *all);
void click(GLOB_T *all);


#endif /* GRAPHIC_H_ */
