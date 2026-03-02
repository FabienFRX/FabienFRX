/*
** EPITECH PROJECT, 2024
** my_exec
** File description:
** azerty
*/

#ifndef AMAZED_H_
    #define AMAZED_H_
    #include "my.h"
    #include "my_printf.h"
    #include "structs.h"
    #include <unistd.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <errno.h>
    #include <features.h>

//main.c
int main(int argc, char **argv);

//error_manager.c
void my_putchar_err(char c);
void my_putstr_error(const char *str);
void my_put_nbr_err(int nb);

//file_reader.c
char *read_file(void);

//parsing1.c
void print_rooms_tag_and_rewrite(void);
int read_num_robots(void);
void remove_comments(void);
void parse_labyrinth(void);

//parsing2.c
void process_room_info(room_info_t *rooms, int *num_rooms,
    int has_start, int has_end);
void check_start_end(room_info_t *current_room, int has_start,
    int has_end, int *num_rooms);
void read_current_room_name(room_info_t *current_room, char *line);
void read_current_room_info(room_info_t *current_room, char *line);

//parsing3.c
void print_room_info(room_info_t *rooms, int num_rooms);
void room_info(room_info_t *rooms, int *num_rooms, int has_start, int has_end);
void detect_end_tag(int *has_end, char *line);
void read_room_info(room_info_t *rooms, int *num_rooms);
void detect_start_tag(int *has_start, char *line);

//parsing4.c
void print_filtered_text(void);

#endif /* !AMAZED_H_ */
