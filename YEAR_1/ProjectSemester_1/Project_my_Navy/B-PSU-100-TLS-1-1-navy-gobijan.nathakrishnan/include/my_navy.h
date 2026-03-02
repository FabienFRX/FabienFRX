/*
** EPITECH PROJECT, 2024
** My_Navy
** File description:
** azerty
*/

#ifndef MY_NAVY_H_
    #define MY_NAVY_H_
    #define _GNU_SOURCE
    #include <signal.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <string.h>
    #include <sys/types.h>
    #include <time.h>
    #include <ucontext.h>
    #include "my.h"


typedef struct map
{
    char **map;
    char **map_e;
} map_t;

typedef struct player
{
    int pid;
    int pid_2;
    int check;
    int attack;
    int x;
    int y;
    int count;
    char *str;
    int defeat;
    int turn;
    int check_eof;
} player_t;

    player_t *p;
    map_t *m;

int disp_help(void);
int error_management_player1(char *filepath);
int error_management_player2(char *filepath);
int error_handling(int argc, char *argv[]);
void signal_handler(int sig, siginfo_t *dope, void *context);
void connection(void);
char *clean_buffer(char *buff);
int boat_counter(char *buff);
char **split_pos(char *buff);
char **openfile(char *filepath);
char **populate_empty_map(char **map);
void display_map(char **map, char **map_e);
int position_check(char **pos);
int is_overlapping(char **pos);
int compare_tab_nbr(int *tab);
int is_in_map(char **pos);
int is_right_size(char **pos);
char **fill_map_w_boats(char **map, char **pos);
char **fill_line(char **map, char **pos, int line, char nb);
char **fill_col(char **map, char **pos, int line, char nb);
int char_to_int1(char c);
int char_to_int2(char c);
int position_check(char **pos);
int compare_tab_nbr(int *tab);
int is_overlapping(char **pos);
char **fill_line(char **map, char **pos, int line, char nb);
char **fill_map_w_boats(char **map, char **pos);
int player_due(char *argv[], map_t *m);
int player_uno(map_t *m);
int hit_counter(char **map);

#endif
