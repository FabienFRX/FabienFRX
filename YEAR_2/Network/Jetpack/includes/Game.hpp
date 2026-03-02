/*
** EPITECH PROJECT, 2024
** Jetpack
** File description:
** GameClient.hpp
*/

#ifndef GAME_CLIENT_HPP
#define GAME_CLIENT_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <string>
#include "Protocol.hpp"

class GameClient {
public:
    GameClient(const std::string& host, int port, bool debug);
    ~GameClient();
    
    void run();
    
private:
    std::string m_host;
    int m_port;
    int m_socket;
    bool m_debug;
    
    std::thread m_networkThread;
    std::mutex m_mutex;
    std::atomic<bool> m_running;
    
    std::vector<std::string> m_mapData;
    float m_playerX;
    float m_playerY;
    int m_playerScore;
    bool m_gameOver;
    std::string m_gameMessage;
    
    void networkLoop();
    void handlePacket(const Packet& packet);
    void sendMove(const std::string& move);
    void debug_log(const std::string& message);
    
    // Drawing methods
    void drawGame(sf::RenderWindow& window);
    void drawMap(sf::RenderWindow& window);
    void drawPlayer(sf::RenderWindow& window);
    void drawUI(sf::RenderWindow& window);
};

#endif
