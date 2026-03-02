/*
** EPITECH PROJECT, 2023
** B-CPE-110 : Shell Boostrap
** File description:
** bootstrap.h
*/

#ifndef BOOTSTRAP_H
    #define BOOTSTRAP_H

typedef enum type {
    CHAR,
    STRING,
    INTEGER,
    PLAYER
} type_t;

typedef struct player_s {
    char *name;
    int lvl;
} player_t;

typedef struct linked_list_s {
    void *data;
    struct linked_list_s *next;
} linked_list_t;

#endif /* BOOTSTRAP_H */
