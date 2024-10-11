/*
** EPITECH PROJECT, 2023
** B-MUL-200-TLS-2-1-myrpg-fabien.fraixanet
** File description:
** inventory.c
*/

#include "../../include/rpg.h"

static map_t *deletenode(map_t *head, int target)
{
    map_t *previous = NULL;
    map_t *current = head;
    map_t *temp;

    if (head != NULL && head->id == target) {
        temp = head;
        head = head->next;
        free(temp);
        return head;
    }
    while (current != NULL && current->id != target) {
        previous = current;
        current = current->next;
    }
    if (current != NULL) {
        previous->next = current->next;
        free(current);
    }
    return head;
}

int *fillinv(int obj_id, int *inv, map_t **map)
{
    for (int i = 0; inv[i] != -1; i++) {
        if (inv[i] == 0) {
            inv[i] = obj_id;
            *map = deletenode(*map, obj_id + 10);
            return inv;
        }
    }
    return inv;
}

static int *check_mouse_on_item(sfVector2i mouse_pos,
    sfVector2f item_size, int *inv)
{
    sfVector2f item_pos;

    for (int i = 0; i <= 27; i++) {
        item_pos = (sfVector2f){(((float)(i % 9)) * 200) + 45,
            ((int)(i / 9) * 200) + 180};
        if ((float)mouse_pos.x > item_pos.x &&
            (float)mouse_pos.x < item_pos.x + item_size.x &&
            (float)mouse_pos.y > item_pos.y &&
            (float)mouse_pos.y < item_pos.y + item_size.y) {
                inv[i] = 0;
                return inv;
        }
    }
    return inv;
}

static int *mouse_pressed_item(window_t *wind, int *inv)
{
    sfVector2i mouse_pos;
    sfVector2f item_size;

    if (sfMouse_isButtonPressed(sfMouseRight)) {
        mouse_pos = sfMouse_getPositionRenderWindow(wind->window);
        item_size = (sfVector2f){220, 220};
        inv = check_mouse_on_item(mouse_pos, item_size, inv);
    }
    return inv;
}

void inventory(window_t *window, sfSprite *inv_sprite, int *inv)
{
    if (sfKeyboard_isKeyPressed(sfKeyE)) {
        while (sfKeyboard_isKeyPressed(sfKeyP) == sfFalse) {
            sfRenderWindow_clear(window->window, sfBlack);
            sfRenderWindow_drawSprite(window->window, inv_sprite, NULL);
            inv = mouse_pressed_item(window, inv);
            window = window_drawspritefromid(inv, window);
            update_view(&(window->window),
                (sfVector2f){11740.0, 10350.0}, window->view);
            sfRenderWindow_display(window->window);
        }
    }
}
