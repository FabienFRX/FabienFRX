/*
** EPITECH PROJECT, 2024
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** client.cpp
*/

#include "Client.hpp"

bool Client::connectToServer(const std::string& host, int port)
{
    struct sockaddr_in serv_addr;
    struct hostent* server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return false;

    server = gethostbyname(host.c_str());
    if (!server) return false;

    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char*)&serv_addr.sin_addr.s_addr, (char*)server->h_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) return false;

    std::string welcome = readLine();
    if (welcome != "WELCOME") return false;

    std::string handshake = "GRAPHIC\n";
    write(sockfd, handshake.c_str(), handshake.size());
    return true;
}

bool Client::waitForMapSize(Map& map)
{
    std::string line;
    while (!(line = readLine()).empty()) {
        if (line.substr(0, 3) == "msz") {
            std::istringstream iss(line);
            std::string cmd;
            int width, height;
            iss >> cmd >> width >> height;
            map.initialize(width, height);
            return true;
        }
    }
    return false;
}

std::string Client::readLine()
{
    char buffer[1024];
    std::string result;
    while (true) {
        ssize_t n = read(sockfd, buffer, sizeof(buffer));
        if (n <= 0) break;
        result.append(buffer, buffer + n);
        size_t pos = result.find('\n');
        if (pos != std::string::npos) {
            std::string line = result.substr(0, pos);
            _data_buffer = result.substr(pos + 1);
            return line;
        }
    }
    return "";
}

void Client::processServerCommands(Map& map) {
    char temp_buffer[4096];
    ssize_t bytes_read = recv(sockfd, temp_buffer, sizeof(temp_buffer) - 1, MSG_DONTWAIT);

    if (bytes_read > 0) {
        temp_buffer[bytes_read] = '\0';
        _data_buffer.append(temp_buffer);
    }

    size_t newline_pos;
    while ((newline_pos = _data_buffer.find('\n')) != std::string::npos) {
        std::string command = _data_buffer.substr(0, newline_pos);
        _data_buffer.erase(0, newline_pos + 1);
        if (!command.empty()) {
            parseCommand(command, map);
        }
    }
}

void Client::parseCommand(const std::string& command, Map& map)
{
    int id;
    int x;
    int y;
    int orientation;
    int level;
    std::istringstream iss(command);
    std::string cmd_type;
    iss >> cmd_type;

    if (cmd_type == "pnw") {
        std::string id_str;
        std::string team;
        if (iss >> id_str >> x >> y >> orientation >> level >> team) {
            if (id_str[0] == '#') {
                id = std::stoi(id_str.substr(1));
                map.addPlayer(id, x, y, static_cast<PlayerOrientation>(orientation), level);
            }
        }
    } else if (cmd_type == "ppo") {
        std::string id_str;
        if (iss >> id_str >> x >> y >> orientation) {
            if (id_str[0] == '#') {
                id = std::stoi(id_str.substr(1));
                map.updatePlayerPosition(id, x, y, static_cast<PlayerOrientation>(orientation));
            }
        }
    }
}
