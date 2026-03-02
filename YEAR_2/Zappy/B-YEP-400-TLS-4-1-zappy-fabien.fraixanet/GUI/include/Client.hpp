/*
** EPITECH PROJECT, 2024
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Client.hpp
*/

#ifndef CLIENT_HPP
#define CLIENT_HPP
#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "Map.hpp"

class Client {
    public:
        bool connectToServer(const std::string& host, int port);
        bool waitForMapSize(Map& map);
        void processServerCommands(Map& map);

    private:
        int sockfd;
        std::string _data_buffer;

        std::string readLine();
        void parseCommand(const std::string& command, Map& map);
};

#endif