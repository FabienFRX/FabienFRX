/*
** EPITECH PROJECT, 2024
** yes
** File description:
** yes
*/


#include "../../include/amazed.h"

void read_current_room_info(room_info_t *current_room, char *line)
{
    char *token = my_str_to_word_array(line, " ");

    if (token == NULL)
        return;
    current_room->id = my_atoi(token);
    token = my_str_to_word_array(NULL, " ");
    if (token == NULL)
        return;
    current_room->x = my_atoi(token);
    token = my_str_to_word_array(NULL, " ");
    if (token == NULL)
        return;
    current_room->y = my_atoi(token);
}

void read_current_room_name(room_info_t *current_room, char *line)
{
    current_room->name = my_strdup(line);
}

void check_start_end(room_info_t *current_room, int has_start,
    int has_end, int *num_rooms)
{
    if (has_start && *num_rooms == 0) {
        current_room->is_start = 1;
    } else {
        current_room->is_start = 0;
    }
    if (has_end && *num_rooms == 0) {
        current_room->is_end = 1;
    } else {
        current_room->is_end = 0;
    }
}

void process_room_info(room_info_t *rooms, int *num_rooms,
    int has_start, int has_end)
{
    char *line = NULL;
    size_t line_size = 0;
    room_info_t current_room;

    while (getline(&line, &line_size, stdin) != -1) {
        if (line[0] == '#' || line[0] == '\n')
            continue;
        read_current_room_info(&current_room, line);
        read_current_room_name(&current_room, line);
        check_start_end(&current_room, has_start, has_end, num_rooms);
        rooms[*num_rooms] = current_room;
        (*num_rooms)++;
    }
    free(line);
}
