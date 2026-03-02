/*
** EPITECH PROJECT, 2024
** yes
** File description:
** yes
*/

#include "../../include/amazed.h"

void detect_start_tag(int *has_start, char *line)
{
    if (line[0] == '#' && line[1] == '#' &&
        line[2] == 's' && line[3] == 't' && line[4] == 'a' &&
        line[5] == 'r' && line[6] == 't') {
        *has_start = 1;
    }
}

void read_room_info(room_info_t *rooms, int *num_rooms)
{
    int has_start = 0;
    int has_end = 0;
    char *line = NULL;
    size_t line_size = 0;

    while (getline(&line, &line_size, stdin) != -1) {
        detect_start_tag(&has_start, line);
        if (has_start) {
            free(line);
            room_info(rooms, num_rooms, has_start, has_end);
            return;
        }
    }
    free(line);
}

void detect_end_tag(int *has_end, char *line)
{
    if (line[0] == '#' && line[1] == '#' &&
        line[2] == 'e' && line[3] == 'n' && line[4] == 'd') {
        *has_end = 1;
    }
}

void room_info(room_info_t *rooms, int *num_rooms, int has_start, int has_end)
{
    char *line = NULL;
    size_t line_size = 0;

    while (getline(&line, &line_size, stdin) != -1) {
        detect_end_tag(&has_end, line);
        if (has_end) {
            free(line);
            return;
        }
    }
    free(line);
}

void print_room_info(room_info_t *rooms, int num_rooms)
{
    for (int i = 0; i < num_rooms; i++) {
        my_printf("%d %d %d %s\n", rooms[i].id, rooms[i].x,
            rooms[i].y, rooms[i].name);
        free(rooms[i].name);
    }
}
