/*
** EPITECH PROJECT, 2024
** B-YEP
** File description:
** error_handling.cpp
*/

#include "Error.hpp"

bool checkError(int argc, char** argv, Map& map, Client& client, int& port, std::string& host)
{
    if (argc != 5 || std::string(argv[1]) != "-p" || std::string(argv[3]) != "-h") {
        std::cerr << "USAGE: ./zappy_gui -p port -h machine" << std::endl;
        return false;
    }
    port = std::stoi(argv[2]);
    host = argv[4];

    if (!client.connectToServer(host, port)) {
        std::cerr << "Failed to connect to server" << std::endl;
        return false;
    }
    if (!client.waitForMapSize(map)) {
        std::cerr << "Failed to get map size" << std::endl;
        return false;
    }
    return true;
}
