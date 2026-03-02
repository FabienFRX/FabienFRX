/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Server
*/

#ifndef SERVER_HPP
#define SERVER_HPP

#include <memory>
#include <chrono>
#include <vector>
#include <string>
#include <poll.h>
#include "Client.hpp"
#include "Game.hpp"
#include "Error.hpp"

class Server {
    public:
        Server(int port, const std::vector<std::string> mapContent, bool debug = false);
        ~Server();

        void launch_server();
        void new_connection(std::vector<struct pollfd> *fds);
        void handle_client_data(int client_fd);

        int get_fd() const;
        int get_port() const;

    private:
        int _fd_server;
        int _port_server;
        std::vector<std::string> _map;
        std::unique_ptr<Game> _game;
        std::chrono::steady_clock::time_point _lastUpdateTime;
        void debug_log(const std::string &message);
        bool _debug;

    public:
        std::vector<Client *> _clients;
};

#endif /* !SERVER_HPP_ */
