/*
** EPITECH PROJECT, 2025
** MY_FTP
** File description:
** Server
*/

#include <cstring>
#include <arpa/inet.h>
#include "ClientHandler.hpp"
#include "Server.hpp"

Server::Server(int port, const std::string &homeDir)
    : m_port(port), m_homeDir(homeDir), m_serverSocket(-1) {}

Server::~Server()
{
    if (m_serverSocket != -1)
    {
        close(m_serverSocket);
    }
    for (size_t i = 1; i < m_clients.size(); i++)
    {
        close(m_clients[i].fd);
    }
}

void Server::setupServer()
{
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverSocket == -1)
    {
        perror("socket creation failed");
        exit(84);
    }

    struct sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(m_port);

    if (bind(m_serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("bind");
        exit(84);
    }

    if (listen(m_serverSocket, MAX_CLIENTS) == -1)
    {
        perror("listen");
        exit(84);
    }

    struct pollfd serverPollfd;
    serverPollfd.fd = m_serverSocket;
    serverPollfd.events = POLLIN;
    m_clients.push_back(serverPollfd);

    std::cout << "Server started on port " << m_port << std::endl;
}

void Server::start()
{
    setupServer();

    while (true)
    {
        if (poll(m_clients.data(), m_clients.size(), -1) == -1)
        {
            perror("poll");
            exit(84);
        }

        for (size_t i = 0; i < m_clients.size(); ++i)
        {
            if (m_clients[i].revents & POLLIN)
            {
                if (m_clients[i].fd == m_serverSocket)
                {
                    handleNewConnection();
                    continue;
                }
                else
                {
                    size_t prevSize = m_clients.size();
                    handleClient(i);
                    if (m_clients.size() == prevSize)
                        ++i;
                    continue;
                }
            }
        }
    }
}

void Server::handleNewConnection()
{
    if (m_clients.size() - 1 >= MAX_CLIENTS)
    {
        std::cerr << "Maximum number of clients reached." << std::endl;
        return;
    }

    struct sockaddr_in clientAddr;
    socklen_t clientSize = sizeof(clientAddr);
    int clientSocket = accept(m_serverSocket, (struct sockaddr *)&clientAddr, &clientSize);
    if (clientSocket == -1)
    {
        perror("accept");
        return;
    }

    std::cout << "New client connected: " << inet_ntoa(clientAddr.sin_addr) << std::endl;

    struct pollfd clientPollfd;
    clientPollfd.fd = clientSocket;
    clientPollfd.events = POLLIN;
    m_clients.push_back(clientPollfd);

    m_handlers.push_back(std::make_unique<ClientHandler>(clientSocket, m_homeDir));
}

bool Server::handleClient(size_t index)
{
    ssize_t handlerIndex = index - 1;

    m_handlers[handlerIndex]->handleClient();
    if (!m_handlers[handlerIndex]->isConnected())
    {
        removeClient(index);
        return false;
    }
    return true;
}

void Server::removeClient(size_t index)
{
    int fd = m_clients[index].fd;

    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    if (getpeername(fd, (struct sockaddr *)&addr, &addr_len) == 0)
    {
        std::cout << "Client disconnected: " << inet_ntoa(addr.sin_addr) << std::endl;
    }
    else
    {
        std::cout << "Client disconnected (fd=" << fd << ")" << std::endl;
    }

    close(fd);
    m_clients.erase(m_clients.begin() + index);
    m_handlers.erase(m_handlers.begin() + index - 1);
}
