/*
** EPITECH PROJECT, 2025
** MY_FTP
** File description:
** CommandParser
*/

#ifndef COMMANDPARSER_HPP
#define COMMANDPARSER_HPP

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <vector>
#include "DataTransfer.hpp"

class ClientHandler;

class CommandParser {
    public:
    void initCommandMap(std::string& currentDir, const std::string& rootDir, int clientSocket, ClientHandler& handler);
    std::string parseCommand(const std::string& command, int clientSocket, std::string& currentDir, const std::string& rootDir, ClientHandler& handler);
    std::vector<std::string> splitCommand(const std::string& command);

    std::string toUpperCase(const std::string& str) const;

    std::string handlePWD(const std::string& currentDir);
    std::string handleCWD(const std::vector<std::string>& args, std::string& currentDir, const std::string& rootDir);
    std::string handleLIST(const std::vector<std::string>& args, const std::string& currentDir, const std::string& rootDir, int dataSocket);
    std::string handleRETR(const std::vector<std::string>& args, int dataSocket, const std::string& currentDir, const std::string& rootDir);
    std::string handleSTOR(const std::vector<std::string>& args, int dataSocket, const std::string& currentDir, const std::string& rootDir);
    std::string handleDELE(const std::vector<std::string>& args, const std::string& currentDir, const std::string& rootDir);
    std::string handleHELP(const std::vector<std::string>& args);
    std::string handleCDUP(std::string& currentDir, const std::string& rootDir);
    std::string handleTYPE(const std::vector<std::string>& args);
    std::string handlePORT(const std::vector<std::string>& args, ClientHandler& handler);

    struct PasvResult {
    std::string response;
    uint16_t port;
};

PasvResult handlePASV(int clientSocket);
std::string handlePASVString(int clientSocket, ClientHandler& handler);

private:
    std::map<std::string, std::function<std::string(const std::vector<std::string>&)>> commandMap;
};
#endif /* !COMMANDPARSER_HPP_ */
