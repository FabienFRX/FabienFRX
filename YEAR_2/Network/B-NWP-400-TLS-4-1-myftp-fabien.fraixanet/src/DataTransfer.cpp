/*
** EPITECH PROJECT, 2025
** MY_FTP
** File description:
** DataTransfer
*/

#include "DataTransfer.hpp"

std::string DataTransfer::sendFile(int clientSocket, const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file)
        return "550 File not found.\r\n";

    char buffer[1024];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        size_t toSend = file.gcount();
        size_t sent = 0;
        while (sent < toSend) {
            ssize_t n = send(clientSocket, buffer + sent, toSend - sent, 0);
            if (n <= 0)
                return "426 Connection closed; transfer aborted.\r\n";
            sent += n;
        }
    }
    return "226 Transfer complete.\r\n";
}

std::string DataTransfer::receiveFile(int clientSocket, const std::string& filePath) {
    std::ofstream file(filePath, std::ios::binary);
    if (!file)
        return "550 Cannot create file.\r\n";

    char buffer[1024];
    int bytesRead;
    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        file.write(buffer, bytesRead);
        if (!file)
            return "451 Requested action aborted. Local error in processing.\r\n";
    }
    file.close();
    return "226 Transfer complete.\r\n";
}
