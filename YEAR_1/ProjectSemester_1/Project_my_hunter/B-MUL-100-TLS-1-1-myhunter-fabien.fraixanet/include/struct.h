/*
** EPITECH PROJECT, 2023
** struct.h
** File description:
** all struct
*/

#ifndef STRUCT_H_
    #define STRUCT_H_

typedef struct PARAM_S {
    sfVideoMode mode;
    sfRenderWindow *window;
    sfEvent event;
} PARAM_T;

typedef struct IMG_S {
    sfTexture *texture;
    sfSprite *sprite;
    sfVector2f scale;
    sfVector2f pos;
    sfVector2f size;
} IMG_T;

typedef struct GLOB_S {
    PARAM_T setting;
    IMG_T *picture;
} GLOB_T;

#endif /* STRUCT_H_ */
