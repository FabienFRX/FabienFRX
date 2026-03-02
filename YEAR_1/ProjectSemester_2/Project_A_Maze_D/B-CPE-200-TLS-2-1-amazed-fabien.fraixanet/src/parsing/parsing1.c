/*
** EPITECH PROJECT, 2024
** yes
** File description:
** yes
*/

#include "../../include/amazed.h"

void parse_labyrinth(void)
{
    int num_robots;
    int num_rooms = 0;
    room_info_t *rooms = malloc(sizeof(room_info_t) * 100);

    if (rooms == NULL)
        return 84;
    num_robots = read_num_robots();
    print_filtered_text();
    print_rooms_tag_and_rewrite();
    read_room_info(rooms, &num_rooms);
    print_room_info(rooms, num_rooms);
    free(rooms);
}

int read_num_robots(void)
{
    char *line = NULL;
    size_t line_size = 0;
    int num_robots;

    if (getline(&line, &line_size, stdin) != -1) {
        num_robots = my_atoi(line);
        if (num_robots == 0 && line[0] != '0')
            return 84;
        my_printf("#number_of_robots\n");
        my_printf("%d\n", num_robots);
        my_printf("#rooms\n");
    } else {
        return 84;
    }
    free(line);
    return num_robots;
}

void print_rooms_tag_and_rewrite(void)
{
    char *line = NULL;
    size_t line_size = 0;
    ssize_t read_bytes;

    while (1) {
        read_bytes = getline(&line, &line_size, stdin);
        if (read_bytes == -1) {
            break;
        }
    }
    free(line);
}
