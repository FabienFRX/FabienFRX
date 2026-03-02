/*
** EPITECH PROJECT, 2023
** my_ls.h
** File description:
** all include of function
*/

#ifndef LIB_INCL_H
    #define LIB_INCL_H
    #include <stdlib.h>
    #include <stdio.h>
    #include <stdbool.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <dirent.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <sys/wait.h>
    #include <signal.h>
    #include "my.h"

typedef struct link_s {
    int data;
    struct link *next;
} link_t;

link_t *create_link(int data);
void print_link(link_t *link);
void connect_links(link_t *link1, link_t *link2);
void print_data_of_connected_links(link_t *link);
link_t build_my_graph(void);
void print_my_graph_data(link_t *graph);
#endif /*LIB_INCL_H*/
