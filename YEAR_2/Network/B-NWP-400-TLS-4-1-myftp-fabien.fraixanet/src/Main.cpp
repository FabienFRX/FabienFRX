/*
** EPITECH PROJECT, 2025
** MY_FTP
** File description:
** Main
*/

#include "Server.hpp"
#include <iostream>
#include <cstdlib>
#include <cctype>

void displayUsage() {
    std::cout << "USAGE: ./myftp port path\n"
              << "port is the port number on which the server socket listens\n"
              << "path is the path to the home directory for the Anonymous user\n";
}

bool isNumber(const std::string& str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc == 2 && argv[1] != nullptr &&
        (std::string(argv[1]) == "-h" || std::string(argv[1]) == "-help")) {
        displayUsage();
        return 84;
    }

    if (argc != 3 || argv[1] == nullptr || argv[2] == nullptr) {
        displayUsage();
        return 84;
    }

    std::string portStr = argv[1];
    std::string homeDir = argv[2];

    if (!isNumber(portStr)) {
        displayUsage();
        return 84;
    }

    int port = std::stoi(portStr);
    if (port <= 0 || port > 65535) {
        std::cerr << "Error: Invalid port number. It must be between 1 and 65535.\n";
        return 84;
    }

    if (chdir(homeDir.c_str()) == -1) {
        std::cerr << "ERROR: bad path given." << std::endl;
        return 84;
    }

    Server server(port, homeDir);
    server.start();

    return 0;
}
