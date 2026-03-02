/*
** EPITECH PROJECT, 2025
** MY_FTP
** File description:
** ClientHandler
*/

#ifndef CLIENTHANDLER_HPP_
#define CLIENTHANDLER_HPP_

#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include "CommandParser.hpp"

class ClientHandler {
public:
    ClientHandler(int clientSocket, const std::string& homeDir);
    ~ClientHandler();

    void handleClient();
    void sendResponse(const std::string& response);
    void processCommand(const std::string& command);
    int setupDataConnection();

    void setConnected(bool value) { m_connected = value; }
    void setActiveMode(const std::string& ip, uint16_t port);
    int connectActiveDataSocket();
    int getDataTransferSocket();
    void setPassiveMode(int serverSock, uint16_t port);



    int getClientSocket() const { return m_clientSocket; }
    bool isAuthenticated() const { return m_authenticated; }
    bool isConnected() const { return m_connected; }
    CommandParser& getParser() { return m_parser; }
    std::string& getCurrentDir() { return m_currentDir; }
    const std::string& getUsername() const { return m_username; }
    const std::string& getRootDir() const { return m_rootDir; }




private:
    int m_clientSocket;
    bool m_authenticated;
    bool m_connected;
    CommandParser m_parser;
    std::string m_commandBuffer;
    std::string m_currentDir;
    std::string m_username;
    std::string m_rootDir;
    int m_dataSocket;
    int m_pasvServerSock = -1;
    uint16_t m_pasvPort = 0;
    bool m_pasvMode = false;
    std::string m_portIp;
    uint16_t m_portPort = 0;
    bool m_portMode = false;
};

#endif /* CLIENTHANDLER_HPP */
