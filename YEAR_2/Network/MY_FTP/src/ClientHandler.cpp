/*
** EPITECH PROJECT, 2025
** MY_FTP
** File description:
** ClientHandler
*/

#include "CommandParser.hpp"
#include "ClientHandler.hpp"

ClientHandler::ClientHandler(int clientSocket, const std::string& homeDir)
: m_clientSocket(clientSocket), m_authenticated(false), m_connected(true),
m_currentDir(homeDir), m_username(""), m_rootDir(homeDir), m_dataSocket(-1) {
    sendResponse("220 Welcome to MyFTP Server\r\n");
}

ClientHandler::~ClientHandler() {
    if (m_clientSocket >= 0) {
        close(m_clientSocket);
        m_clientSocket = -1;
    }
    if (m_dataSocket >= 0) {
        close(m_dataSocket);
        m_dataSocket = -1;
    }
}

int ClientHandler::setupDataConnection() {
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == -1) {
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        close(serverSock);
        return -1;
    }

    if (listen(serverSock, 1) == -1) {
        close(serverSock);
        return -1;
    }

    struct pollfd pfd;
    pfd.fd = serverSock;
    pfd.events = POLLIN;
    int pollRes = poll(&pfd, 1, 10000);
    if (pollRes <= 0) {
        close(serverSock);
        return -1;
    }

    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int dataSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientLen);
    close(serverSock);
    if (dataSock == -1) {
        return -1;
    }
    return dataSock;
}

void ClientHandler::handleClient() {
    char buffer[1024];
    ssize_t bytesRead = recv(m_clientSocket, buffer, sizeof(buffer) -1, 0);
    if (bytesRead <= 0) {
        setConnected(false);
        return;
        }

    buffer[bytesRead] = '\0';
    std::string receivedData(buffer);
    m_commandBuffer += receivedData;

    if (m_commandBuffer.size() > 4096) {
        sendResponse("500 Command too long.\r\n");
        m_commandBuffer.clear();
        return;
    }

    size_t start = 0;
    while (start < m_commandBuffer.size()) {
        size_t end = m_commandBuffer.find_first_of("\r\n", start);
        if (end == std::string::npos)
            break;

        std::string cmd = m_commandBuffer.substr(start, end - start);
        if (!cmd.empty()) {
            std::cout << "Processing command: [" << cmd << "]" << std::endl;
            processCommand(cmd);
        }

        while (end < m_commandBuffer.size() &&
            (m_commandBuffer[end] == '\r' || m_commandBuffer[end] == '\n'))
            ++end;
        start = end;
    }
    m_commandBuffer = m_commandBuffer.substr(start);
}

void ClientHandler::sendResponse(const std::string& response) {
    std::cout << "Sending response: " << response;
    ssize_t sent = send(m_clientSocket, response.c_str(), response.size(), 0);
    if (sent == -1) {
        std::cerr << "send error" << std::endl;
        m_connected = false;
    }
}

void ClientHandler::processCommand(const std::string& command) {
    std::vector<std::string> args = m_parser.splitCommand(command);
    if (args.empty()) {
        sendResponse("500 Syntax error, command unrecognized.\r\n");
        return;
    }

    std::string cmd = m_parser.toUpperCase(args[0]);
    if (!m_authenticated) {
        if (cmd == "USER") {
            if (args.size() < 2) {
                sendResponse("501 Syntax error in parameters or arguments.\r\n");
                return;
            }
            m_username = args[1];
            if (m_parser.toUpperCase(m_username) != "ANONYMOUS") {
                sendResponse("530 Only anonymous login supported.\r\n");
                m_username.clear();
                return;
            }
            sendResponse("331 Please specify your password.\r\n");
            return;
        }
        else if (cmd == "PASS") {
            if (m_username.empty()) {
                sendResponse("332 User not specified.\r\n");
                return;
            }
            if (m_parser.toUpperCase(m_username) != "ANONYMOUS") {
                sendResponse("530 User unknown.\r\n");
                return;
            }
            sendResponse("230 Login successful.\r\n");
            m_authenticated = true;
            m_username.clear();
            return;
        }
        else if (cmd == "QUIT") {
            sendResponse("221 Closing client...\r\n");
            setConnected(false);
            return;
        }
        else if (cmd == "NOOP") {
            sendResponse("530 Login with USER and PASS first.\r\n");
            return;
        }
        else {
            sendResponse("530 Please login with USER and PASS.\r\n");
            return;
        }
    }
    if (cmd == "QUIT") {
        sendResponse("221 Closing client...\r\n");
        setConnected(false);
        return;
    }

    
    m_parser.initCommandMap(m_currentDir, m_rootDir, m_clientSocket, *this);
    std::string response = m_parser.parseCommand(command, m_clientSocket, m_currentDir, m_rootDir, *this);
    if (!response.empty())
        sendResponse(response);
}

void ClientHandler::setActiveMode(const std::string& ip, uint16_t port) {
    m_portIp = ip;
    m_portPort = port;
    m_portMode = true;
}

int ClientHandler::connectActiveDataSocket() {
    if (!m_portMode)
        return -1;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        return -1;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_portPort);
    inet_pton(AF_INET, m_portIp.c_str(), &addr.sin_addr);
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        close(sock);
        return -1;
    }
    return sock;
}

int ClientHandler::getDataTransferSocket() {
    if (m_portMode) {
        int sock = connectActiveDataSocket();
        m_portMode = false;
        return sock;
    } else if (m_pasvMode && m_pasvServerSock != -1) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int dataSock = accept(m_pasvServerSock, (struct sockaddr*)&clientAddr, &clientLen);
        close(m_pasvServerSock);
        m_pasvServerSock = -1;
        m_pasvMode = false;
        return dataSock;
    }
    return -1;
}

void ClientHandler::setPassiveMode(int serverSock, uint16_t port) {
    if (m_pasvServerSock != -1)
        close(m_pasvServerSock);
    m_pasvServerSock = serverSock;
    m_pasvPort = port;
    m_pasvMode = true;
}
