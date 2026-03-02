/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Server header file
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <poll.h>
    #include <stdbool.h>
    #include <getopt.h>
    #include <time.h>
    #include <errno.h>
    #include <float.h>
    #include <strings.h>
    #include "game.h"

    #define MAX_CLIENTS 1024
    #define UNUSED(x) (void)(x)

typedef enum client_state_e {
    AWAITING_TEAM_NAME,
    PROCESSING_COMMANDS
} client_state_t;

typedef struct arguments_s {
    int port;
    int width;
    int height;
    char **team_names;
    int clients_nb;
    int freq;
} arguments_t;

typedef struct egg_s {
    int id;
    int x;
    int y;
    char team_name[256];
    double creation_time;
    bool is_hatched;
} egg_t;

typedef struct client_info_s {
    int fd;
    client_state_t state;
    bool is_graphic;
    int player_id;
    char team_name[256];
} client_info_t;

typedef struct evolution_site_s {
    int x;
    int y;
    int level;
    char team_name[256];
    int coordinator_id;
    double creation_time;
    int players_ready;
    bool is_active;
} evolution_site_t;

typedef struct evolution_request_s {
    int level;
    int x;
    int y;
    char team_name[256];
    int coordinator_id;
    double creation_time;
    int players_ready;
    int is_active;
} evolution_request_t;

typedef struct server_s {
    int port;
    int server_fd;
    struct pollfd fds[MAX_CLIENTS + 1];
    client_info_t clients[MAX_CLIENTS + 1];
    int client_count;
    arguments_t *args;
    map_t *map;
    player_t **players;
    int player_count;
    double last_resource_spawn;
    egg_t **eggs;
    int egg_count;
    int next_egg_id;
    int game_won;
    double win_time;
    evolution_site_t evolution_sites[MAX_CLIENTS];
    int evolution_site_count;
    double last_evolution_check;
    evolution_request_t evolution_requests[10];
    int evolution_request_count;
} server_t;

bool parse_args(int ac, char **av, arguments_t *args);
bool handle_teams_arg(char **av, int *i, arguments_t *args, int ac);

void init_server(server_t *server, arguments_t *args);
void run_server(server_t *server);
void remove_client(server_t *server, int i);
void error_exit(const char *msg);

void print_string_details(const char *label, const char *str);
player_t *get_player_by_fd(server_t *server, int fd);

void handle_client_command(server_t *server, int client_index,
                           const char *cmd);
void execute_timed_command(server_t *server, player_t *player);
int is_immediate_command(const char *cmd);

void handle_inventory(server_t *server, int client_index);
void handle_look(server_t *server, int client_index);
void handle_forward(server_t *server, int client_index);
void handle_left(server_t *server, int client_index);
void handle_right(server_t *server, int client_index);

void handle_take(server_t *server, int client_index, const char *cmd);
void handle_set(server_t *server, int client_index, const char *cmd);

void handle_broadcast(server_t *server, int client_index, const char *text);
void handle_incantation(server_t *server, int client_index);
int start_incantation(server_t *server, int client_index);
void end_incantation(server_t *server, int client_index);
void handle_connect_nbr(server_t *server, int client_index);
void handle_fork(server_t *server, int client_index);
void handle_eject(server_t *server, int client_index);

void process_timed_actions(server_t *server);
void update_player_food(server_t *server, player_t *player);
void check_all_players_food(server_t *server);

void spawn_initial_resources(server_t *server);
void spawn_periodic_resources(server_t *server);

void init_egg_system(server_t *server);
void cleanup_egg_system(server_t *server);
void create_initial_eggs(server_t *server);
int count_team_eggs(server_t *server, const char *team_name);
egg_t *find_oldest_egg(server_t *server, const char *team_name);
void add_egg_to_server(server_t *server, egg_t *egg);
egg_t *create_egg_at_position(server_t *server, const char *team_name,
                              int x, int y);

player_t *create_player_from_egg(server_t *server, int client_fd,
                                 const char *team_name);
player_t *create_player_direct(server_t *server, int client_fd,
                               const char *team_name);
int count_team_players(server_t *server, const char *team_name);
void create_player_for_client(server_t *server, int i, const char *team_name);
char *get_player_team_name(server_t *server, int player_id);

void handle_handshake(server_t *server, int i, char *team_name);

void init_server_components(server_t *server);

char *check_win_condition(server_t *server);
void broadcast_win(server_t *server, const char *winning_team);
void check_and_handle_win(server_t *server);
void reset_game_state(server_t *server);

void init_command_time(player_t *player, double duration);
int is_command_finished(player_t *player);
double get_remaining_time(player_t *player);
double get_current_time(void);
void process_timed_actions(server_t *server);
void execute_finished_command(server_t *server, player_t *player);

#endif
