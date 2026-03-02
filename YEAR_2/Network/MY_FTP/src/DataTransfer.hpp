/*
** EPITECH PROJECT, 2025
** MY_FTP
** File description:
** DataTransfer
*/

#ifndef DATATRANSFER_HPP_
#define DATATRANSFER_HPP_

#include <iostream>
#include <string>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class DataTransfer
{
public:
    static std::string sendFile(int clientSocket, const std::string &filePath);
    static std::string receiveFile(int clientSocket, const std::string &filePath);
};

#endif /* !DATATRANSFER_HPP_ */
