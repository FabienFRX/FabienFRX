/*
** EPITECH PROJECT, 2024
** B-PSU-200-TLS-2-1-minishell1-gobijan.nathakrishnan [WSL: Debian]
** File description:
** structs
*/

#ifndef STRUCTS_H_
    #define STRUCTS_H_

typedef struct RoomInfo {
    int id;
    int x;
    int y;
    char *name;
    int is_start;
    int is_end;
} room_info_t;

typedef struct Tunnel {
    int room_id1;
    int room_id2;
} tunnel_t;
#endif /* !STRUCTS_H_ */
