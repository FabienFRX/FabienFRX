/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Server main loop and initialization
*/

#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
#include <unistd.h>
#include "server.h"

static void handle_new_connection(server_t *server)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server->server_fd,
                          (struct sockaddr *)&client_addr, &client_len);
    int i = 0;

    if (client_fd < 0)
        return;
    if (server->client_count >= MAX_CLIENTS) {
        dprintf(client_fd, "Server is full.\n");
        close(client_fd);
        return;
    }
    i = server->client_count + 1;
    server->fds[i].fd = client_fd;
    server->fds[i].events = POLLIN;
    server->clients[i].fd = client_fd;
    server->clients[i].state = AWAITING_TEAM_NAME;
    server->clients[i].is_graphic = false;
    server->client_count++;
    printf("New client connected (fd: %d, total: %d)\n", client_fd, server->client_count);
    dprintf(client_fd, "WELCOME\n");
}

static void handle_client_data(server_t *server, int i)
{
    char buffer[1024];
    ssize_t n = read(server->fds[i].fd, buffer, sizeof(buffer) - 1);
    static char client_buffers[MAX_CLIENTS][4096] = {0};

    if (n <= 0) {
        printf("Client disconnected (fd: %d)\n", server->fds[i].fd);
        remove_client(server, i);
        return;
    }
    
    buffer[n] = '\0';
    strncat(client_buffers[i], buffer, sizeof(client_buffers[i]) - strlen(client_buffers[i]) - 1);
    
    char *line_start = client_buffers[i];
    char *newline_pos;
    
    while ((newline_pos = strchr(line_start, '\n')) != NULL) {
        *newline_pos = '\0';
        
        if (server->clients[i].state == AWAITING_TEAM_NAME) {
            handle_handshake(server, i, line_start);
        } else {
            handle_client_command(server, i, line_start);
        }
        
        line_start = newline_pos + 1;
    }
    
    if (line_start != client_buffers[i]) {
        memmove(client_buffers[i], line_start, strlen(line_start) + 1);
    }
}

static void process_server_events(server_t *server, int poll_result)
{
    int i = 0;

    if (poll_result <= 0)
        return;
    if (server->fds[0].revents & POLLIN) {
        handle_new_connection(server);
    }
    for (i = 1; i <= server->client_count; i++) {
        if (server->fds[i].revents & POLLIN) {
            handle_client_data(server, i);
        }
    }
}

void run_server(server_t *server)
{
    int poll_timeout = 1000 / server->args->freq;
    int poll_result = 0;

    while (true) {
        check_and_handle_win(server);
        process_timed_actions(server);
        check_all_players_food(server);
        spawn_periodic_resources(server);
        poll_result = poll(server->fds, server->client_count + 1,
                          poll_timeout);
        if (poll_result < 0) {
            error_exit("poll");
        }
        process_server_events(server, poll_result);
        if (server->game_won && server->client_count == 0) {
            reset_game_state(server);
        }
        
        usleep(1000);
    }
}

