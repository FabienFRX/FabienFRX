/*
** EPITECH PROJECT, 2025
** MY_FTP
** File description:
** Server
*/

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <memory>
#include "ClientHandler.hpp"

class Server
{
public:
    Server(int port, const std::string &homeDir);
    ~Server();

    void start();

private:
    int m_port;
    std::string m_homeDir;
    int m_serverSocket;
    static const size_t MAX_CLIENTS = 10;
    std::vector<pollfd> m_clients;
    std::vector<std::unique_ptr<ClientHandler>> m_handlers;

    void setupServer();
    void handleNewConnection();
    bool handleClient(size_t index);
    void removeClient(size_t index);
};

#endif /* SERVER_HPP */
