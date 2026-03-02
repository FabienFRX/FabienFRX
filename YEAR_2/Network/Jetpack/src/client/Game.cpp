/*
** EPITECH PROJECT, 2024
** Jetpack
** File description:
** GameClient.cpp
*/

#include "Game.hpp"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

GameClient::GameClient(const std::string& host, int port, bool debug)
    : m_host(host), m_port(port), m_socket(-1), m_debug(debug),
      m_running(false), m_playerX(0), m_playerY(0), m_playerScore(0),
      m_gameOver(false)
{
}

GameClient::~GameClient()
{
    m_running = false;
    if (m_networkThread.joinable()) {
        m_networkThread.join();
    }
    
    if (m_socket != -1) {
        close(m_socket);
    }
}

void GameClient::debug_log(const std::string& message)
{
    if (m_debug) {
        std::cout << "[DEBUG] " << message << std::endl;
    }
}

void GameClient::run()
{
    try {
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket == -1) {
            throw ClientError("Error: Failed to create socket");
        }

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(m_port);

        if (inet_pton(AF_INET, m_host.c_str(), &server_addr.sin_addr) <= 0) {
            throw ClientError("Error: Invalid address or unsupported address format");
        }

        if (connect(m_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            throw ClientError("Error: Failed to connect to server");
        }

        debug_log("Connected to server");

        m_running = true;
        m_networkThread = std::thread(&GameClient::networkLoop, this);

        sf::RenderWindow window(sf::VideoMode(800, 600), "Jetpack Joyride");
        window.setFramerateLimit(60);

        while (window.isOpen() && m_running) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            window.clear(sf::Color::Black);
            drawGame(window);
            window.display();
        }

        m_running = false;
        if (m_networkThread.joinable()) {
            m_networkThread.join();
        }
    } catch (const ClientError &e) {
        std::cerr << e.what() << std::endl;
    }
}

void GameClient::networkLoop()
{
    std::vector<uint8_t> buffer(1024);
    
    while (m_running) {
        // Receive data from server
        int bytesRead = recv(m_socket, buffer.data(), buffer.size(), 0);
        
        if (bytesRead <= 0) {
            debug_log("Server disconnected");
            m_running = false;
            break;
        }
        
        debug_log("Received " + std::to_string(bytesRead) + " bytes");
        
        try {
            Packet packet = Packet::deserialize(std::vector<uint8_t>(buffer.begin(), buffer.begin() + bytesRead));
            handlePacket(packet);
        } catch (const std::exception& e) {
            debug_log("Failed to parse packet: " + std::string(e.what()));
        }
    }
}

void GameClient::handlePacket(const Packet& packet)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    switch (packet.type) {
        case PacketType::HELLO: {
            debug_log("Received HELLO packet");
            std::string message = bytesToString(packet.payload);
            debug_log("Message: " + message);
            break;
        }
        case PacketType::MAP_DATA: {
            debug_log("Received MAP_DATA packet");
            std::string mapStr = bytesToString(packet.payload);
            
            // Split into lines
            m_mapData.clear();
            size_t pos = 0;
            while ((pos = mapStr.find("\n")) != std::string::npos) {
                std::string line = mapStr.substr(0, pos);
                if (!line.empty()) {
                    m_mapData.push_back(line);
                }
                mapStr.erase(0, pos + 1);
            }
            if (!mapStr.empty()) {
                m_mapData.push_back(mapStr);
            }
            
            debug_log("Map loaded with " + std::to_string(m_mapData.size()) + " rows");
            break;
        }
        case PacketType::PLAYER_POS: {
            std::string posStr = bytesToString(packet.payload);
            size_t commaPos = posStr.find(",");
            if (commaPos != std::string::npos) {
                m_playerX = std::stof(posStr.substr(0, commaPos));
                m_playerY = std::stof(posStr.substr(commaPos + 1));
            }
            break;
        }
        case PacketType::PLAYER_SCORE: {
            m_playerScore = std::stoi(bytesToString(packet.payload));
            debug_log("Score updated: " + std::to_string(m_playerScore));
            break;
        }
        case PacketType::PLAYER_DEATH: {
            debug_log("Player died");
            break;
        }
        case PacketType::GAME_START: {
            debug_log("Game started");
            break;
        }
        case PacketType::GAME_OVER: {
            debug_log("Game over");
            m_gameOver = true;
            m_gameMessage = bytesToString(packet.payload);
            break;
        }
        default:
            debug_log("Unknown packet type: " + std::to_string(static_cast<int>(packet.type)));
    }
}

void GameClient::sendMove(const std::string& move)
{
    Packet packet;
    packet.type = PacketType::PLAYER_MOVE;
    packet.payload = stringToBytes(move);
    
    std::vector<uint8_t> serialized = packet.serialize();
    send(m_socket, serialized.data(), serialized.size(), 0);
    
    debug_log("Sent move: " + move);
}

void GameClient::drawGame(sf::RenderWindow& window)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    drawMap(window);
    drawPlayer(window);
    drawUI(window);
}

void GameClient::drawMap(sf::RenderWindow& window)
{
    const float tileSize = 30.0f;
    
    for (size_t y = 0; y < m_mapData.size(); ++y) {
        const std::string& row = m_mapData[y];
        for (size_t x = 0; x < row.length(); ++x) {
            sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
            tile.setPosition(x * tileSize, y * tileSize);
            
            switch (row[x]) {
                case '_':  // Empty space
                    continue;
                case 'c':  // Coin
