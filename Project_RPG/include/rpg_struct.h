/*
** EPITECH PROJECT, 2023
** my_paint.h
** File description:
** all include of function
*/

#ifndef MY_PAINT_H
    #define MY_PAINT_H

    #include <SFML/System/Export.h>
    #include <SFML/System/Vector2.h>

    #include <SFML/Window/Window.h>
    #include <SFML/Window/Event.h>
    #include <SFML/Window.h>

    #include <SFML/Graphics.h>
    #include <SFML/Graphics/Rect.h>
    #include <SFML/Graphics/Types.h>
    #include <SFML/Graphics/Export.h>
    #include <SFML/Graphics/Color.h>
    #include <SFML/Graphics/Transform.h>

    #include <stdlib.h>
    #include <stdbool.h>
    #include <unistd.h>
    #include <stddef.h>
    #include <math.h>
    #include <string.h>


typedef struct object_s {
    int id_obj;
    char *filename;
//    struct object *next;
} object_t;

typedef struct enemies_s {
    sfRectangleShape *hitbox;
    sfSprite *sprite;
    sfVector2f position;
    sfVector2f size;
    int id;
    struct enemies_s *next;
} enemies_t;

typedef struct player_s {
    char *name;
    float frameDuration;
    float elapsedTime;
    sfClock *clock;
    sfVector2f position;
    sfTexture *texture;
    sfSprite *sprite;
    sfRectangleShape *hitbox;
    sfVector2f h_position;
    sfVector2f h_size;
    sfIntRect textureRect;
    int *inv;
    enemies_t *enemies;
} player_t;

//typedef struct player_s {
//    char *name;
//    sfVector2f position;
//    sfTexture *texture;
//    sfSprite *sprite;
//    sfRectangleShape *hitbox;
//    sfVector2f h_size;
//    int *inv;
//    enemies_t *enemies;
//} player_t;

typedef struct window_s {
    char *title;
    sfRenderWindow *window;
    sfView *view;
    sfVideoMode mode;
    sfVector2f size_w;
    sfSprite *background;
    sfSprite **sprites;
} window_t;

typedef struct map_s {
    sfRectangleShape *walls;
    sfVector2f position;
    sfVector2f size;
    int id;
    struct map_s *next;
    struct map_s *prev;
} map_t;

typedef struct start_s {
    map_t *start;
    enemies_t *first;
} start_t;

typedef struct walls_s {
    sfVector2f size;
    sfVector2f position;
    int id;
} walls_t;

typedef struct sprites_s {
    char *filename;// from the binary (command : find -name "filename")
    sfVector2f scale;//{1, 1} == same scale than the file
    sfVector2f origin; //{0, 0} == without origin
    sfVector2f position;
    char *mnémonique;
} sprites_t;

#endif/*MY_PAINT_H*/
