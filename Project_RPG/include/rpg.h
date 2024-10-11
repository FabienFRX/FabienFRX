/*
** EPITECH PROJECT, 2024
** B-MUL-200-TLS-2-1-mypaint-andi.berard
** File description:
** my_paint
*/

#ifndef MY_PAINT_H_
    #define MY_PAINT_H_

    #include "rpg_struct.h"
    #include "my.h"

sfTexture *gen_background(char const *filepath);
sfRectangleShape *create_rect(sfVector2f size,
    sfVector2f position, sfColor color);
sfRectangleShape *create_rectnorigin(sfVector2f size,
    sfVector2f position, sfColor color);
sfCircleShape *create_circle(float radius, sfVector2f position, sfColor color);
sfSprite *create_sprite(sfTexture *texture, sfVector2f position);

//window
//sfRenderWindow *createwindow(int width, int height, const char *title);
window_t *init_window(void);
void start(window_t *window, player_t *player, map_t *map, bool pause);

//sprites
sfSprite **create_sprite_array(void);
int sprite_id(char *mnémonique);
extern sprites_t sprite_tab[];

//view
sfView *create_view(sfRenderWindow **window, sfVector2f player_pos);
sfView *update_view(sfRenderWindow **window, sfVector2f player_pos,
    sfView *view);

//player
player_t *create_player(player_t *player);
void check_move(player_t *player);
void update_player(player_t *player, window_t *window);
int collision(map_t *m, player_t *p);
//animation
void moovedown(player_t *player, float frame_duration);
void mooveup(player_t *player, float frame_duration);
void mooveright(player_t *player, float frame_duration);
void mooveleft(player_t *player, float frame_duration);
void iddle(player_t *player, float frame_duration);

//inventory
int *init_inventory(void);
void inventory(window_t *window, sfSprite *inv_sprite, int *inv);
void init_object(window_t *window);
window_t *window_drawspritefromid(int *inv, window_t *window);
int *fillinv(int obj_id, int *inv, map_t **map);
extern object_t obj_tab[];

//map
map_t *pars_node(map_t *map, window_t *wind);
map_t *push(sfVector2f size, sfVector2f position, int id);
extern walls_t walls_tab[];

//enemies
enemies_t *init_enemies(sfIntRect spawn_area, sfSprite **sprites);
sfIntRect spawn_id(int colli);

//collision id:
/*
    1 = walls
    31 -> 34 = pnj
    11 -> 14 = item_inv
*/
#endif /* !MY_PAINT_H_ */
