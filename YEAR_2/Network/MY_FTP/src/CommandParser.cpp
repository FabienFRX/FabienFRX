/*
** EPITECH PROJECT, 2025
** MY_FTP
** File description:
** CommandParser
*/

#include "ClientHandler.hpp"
#include "CommandParser.hpp"

static const std::map<std::string, std::string> helpMessages = {
    {"CDUP", "CDUP\nChange working directory to parent directory\n"},
    {"CWD", "CWD path\nChange working directory\n"},
    {"DELE", "DELE file\nDelete a file\n"},
    {"HELP", "HELP [cmd]\nIf cmd, display help for this one else list all cmds\n"},
    {"NOOP", "NOOP\nNO Operation\n"},
    {"PASS", "PASS psswd\nSpecify password for authentication\n"},
    {"PWD", "PWD\nPrint working directory\n"},
    {"QUIT", "QUIT\nDisconnection\n"},
    {"USER", "USER username\nSpecify username for authentication\n"},
    {"LIST", "LIST [dir]\nList files in the directory (default ./)\n"},
    {"PORT", "PORT h1,h2,h3,h4,p1,p2\nEnable active mode for data connection\n"}
};

std::string CommandParser::toUpperCase(const std::string& str) const {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

void CommandParser::initCommandMap(std::string& currentDir, const std::string& rootDir, int clientSocket, ClientHandler& handler) {
    commandMap = {
        {"PWD", [this, &currentDir](const std::vector<std::string>&) {
            return handlePWD(currentDir);
        }},
        {"CWD", [this, &currentDir, &rootDir](const std::vector<std::string>& args) {
            return handleCWD(args, currentDir, rootDir);
        }},
        {"LIST", [this, &currentDir, &rootDir, &handler](const std::vector<std::string>& args) {
            int dataSock = handler.getDataTransferSocket();
            if (dataSock == -1)
                return std::string("425 Can't open data connection.\r\n");

            handler.sendResponse("150 Here comes the directory listing.\r\n");
            std::string result = handleLIST(args, currentDir, rootDir, dataSock);
            close(dataSock);
            handler.sendResponse("226 Directory send OK.\r\n");
            return result;
        }},
        {"RETR", [this, &currentDir, &rootDir, &handler](const std::vector<std::string>& args) {
            int dataSock = handler.getDataTransferSocket();
            if (dataSock == -1)
                return std::string("425 Can't open data connection.\r\n");

            handler.sendResponse("150 Opening data connection.\r\n");
            std::string result = handleRETR(args, dataSock, currentDir, rootDir);
            close(dataSock);
            handler.sendResponse("226 Transfer complete.\r\n");
            return result;
        }},
        {"STOR", [this, &currentDir, &rootDir, &handler](const std::vector<std::string>& args) {
            int dataSock = handler.getDataTransferSocket();
            if (dataSock == -1)
                return std::string("425 Can't open data connection.\r\n");

            handler.sendResponse("150 Opening data connection.\r\n");
            std::string result = handleSTOR(args, dataSock, currentDir, rootDir);
            close(dataSock);
            handler.sendResponse("226 Transfer complete.\r\n");
            return result;
        }},
        {"DELE", [this, &currentDir, &rootDir](const std::vector<std::string>& args) {
            return handleDELE(args, currentDir, rootDir);
        }},
        {"HELP", [this](const std::vector<std::string>& args) {
            return handleHELP(args);
        }},
        {"CDUP", [this, &currentDir, &rootDir](const std::vector<std::string>&) {
            return handleCDUP(currentDir, rootDir);
        }},
        {"NOOP", [](const std::vector<std::string>&) {
            return std::string("200 NOOP command successful.\r\n");
        }},
        {"QUIT", [](const std::vector<std::string>&) {
            return std::string("221 Goodbye.\r\n");
        }},
        {"USER", [this](const std::vector<std::string>& args) {
            if (args.size() >= 2 && toUpperCase(args[1]) == "ANONYMOUS") {
                return std::string("331 User name okay, need password.\r\n");
            } else {
                return std::string("530 Only anonymous login supported.\r\n");
            }
        }},
        {"PASV", [this, clientSocket, &handler](const std::vector<std::string>&) {
            return handlePASVString(clientSocket, handler);
        }},
        {"PORT", [this, &handler](const std::vector<std::string>& args) {
            return handlePORT(args, handler);
        }},
        {"TYPE", [this](const std::vector<std::string>& args) {
            return (handleTYPE(args));
        }},
    };
}

bool isPathAllowed(const std::filesystem::path& path, const std::filesystem::path& root) {
    std::error_code ec;
    auto canonicalPath = std::filesystem::weakly_canonical(path, ec);
    if (ec) return false;
    return !std::filesystem::relative(canonicalPath, root).string().starts_with("..");
}

std::string CommandParser::parseCommand(const std::string& command, int clientSocket, std::string& currentDir, const std::string& rootDir, ClientHandler& handler) {
    std::string cleanCommand = command;
    cleanCommand.erase(std::remove(cleanCommand.begin(), cleanCommand.end(), '\r'), cleanCommand.end());
    cleanCommand.erase(std::remove(cleanCommand.begin(), cleanCommand.end(), '\n'), cleanCommand.end());

    std::vector<std::string> args = splitCommand(cleanCommand);
    if (args.empty())
        return "500 Syntax error, command unrecognized.\r\n";

    std::string cmd = toUpperCase(args[0]);

    initCommandMap(currentDir, rootDir, clientSocket, handler);

    auto it = commandMap.find(cmd);
    if (it != commandMap.end()) {
        return it->second(args);
    }
    else {
        return "500 Syntax error, command unrecognized.\r\n";
    }
}

std::string CommandParser::handlePASVString(int clientSocket, ClientHandler& handler) {
    PasvResult res = handlePASV(clientSocket);
    if (res.port == 0)
        return res.response;

    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == -1)
        return "425 Can't open passive socket.\r\n";

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(res.port);

    if (bind(serverSock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        close(serverSock);
        return "425 Can't bind passive socket.\r\n";
    }
    if (listen(serverSock, 1) == -1) {
        close(serverSock);
        return "425 Can't listen on passive socket.\r\n";
    }

    handler.setPassiveMode(serverSock, res.port);

    return res.response;
}

std::vector<std::string> CommandParser::splitCommand(const std::string& command) {
    std::vector<std::string> tokens;
    std::istringstream stream(command);
    std::string token;

    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string CommandParser::handlePWD(const std::string& currentDir) {
    if (currentDir.empty())
        return "550 Failed to get current directory.\r\n";
    return "257 \"" + currentDir + "\" is the current directory.\r\n";
}

std::string CommandParser::handleCWD(const std::vector<std::string>& args, std::string& currentDir, const std::string& rootDir) {
    if (args.size() < 2) {
        return "501 Syntax error in parameters or arguments.\r\n";
    }
    try {
        std::filesystem::path root = rootDir;
        std::filesystem::path newPath;
        if (!args[1].empty() && args[1][0] == '/') {
            newPath = args[1];
        } else {
            newPath = std::filesystem::path(currentDir) / args[1];
        }
        std::error_code ec;
        auto canonicalNewPath = std::filesystem::weakly_canonical(newPath, ec);
        if (ec || !std::filesystem::exists(canonicalNewPath) || !std::filesystem::is_directory(canonicalNewPath)) {
            return "550 Failed to change directory.\r\n";
        }
        if (!isPathAllowed(canonicalNewPath, root)) {
            return "550 Permission denied.\r\n";
        }
        currentDir = canonicalNewPath.string();
        return "250 Directory successfully changed.\r\n";
    } catch (const std::exception& e) {
        return "550 Failed to change directory (exception).\r\n";
    }
}

std::string CommandParser::handleLIST(const std::vector<std::string>& args, const std::string& currentDir, const std::string& rootDir, int dataSocket) {
    try {
        std::filesystem::path root = rootDir;
        std::string listDir = currentDir;

        if (args.size() >= 2) {
            std::filesystem::path requested = std::filesystem::absolute(args[1]);
            std::error_code ec;
            auto canonicalRequested = std::filesystem::weakly_canonical(requested, ec);
            if (ec || !std::filesystem::exists(canonicalRequested) || !std::filesystem::is_directory(canonicalRequested)) {
                return "550 Directory not found.\r\n";
            }
            if (!isPathAllowed(canonicalRequested, root)) {
                return "550 Permission denied.\r\n";
            }
            listDir = canonicalRequested.string();
        }

        std::string fileList;
        for (const auto& entry : std::filesystem::directory_iterator(listDir)) {
            fileList += entry.path().filename().string();
            if (entry.is_directory())
                fileList += "/";
            fileList += "\r\n";
        }

        if (!fileList.empty()) {
            send(dataSocket, fileList.c_str(), fileList.size(), 0);
        }
        return "";
    } catch (const std::exception& e) {
        return "451 Requested action aborted.\r\n";
    }
}

std::string CommandParser::handleRETR(const std::vector<std::string>& args, int dataSocket, const std::string& currentDir, const std::string& rootDir) {
    if (args.size() < 2)
        return "501 Syntax error in parameters.\r\n";
    try {
        std::filesystem::path root = rootDir;
        std::filesystem::path filePath = std::filesystem::absolute(currentDir + "/" + args[1]);
        std::error_code ec;
        auto canonicalFile = std::filesystem::weakly_canonical(filePath, ec);
        if (ec || !std::filesystem::exists(canonicalFile) || !std::filesystem::is_regular_file(canonicalFile)) {
            return "550 File not found.\r\n";
        }
        if (!isPathAllowed(canonicalFile, root)) {
            return "550 Permission denied.\r\n";
        }

        std::string result = DataTransfer::sendFile(dataSocket, canonicalFile.string());
        if (result != "226 Transfer complete.\r\n")
            return result;
        return "";
    } catch (const std::exception& e) {
        return "550 Failed to retrieve file (exception).\r\n";
    }
}

std::string CommandParser::handleSTOR(const std::vector<std::string>& args, int dataSocket, const std::string& currentDir, const std::string& rootDir) {
    if (args.size() < 2)
        return "501 Syntax error in parameters.\r\n";
    try {
        std::filesystem::path root = rootDir;
        std::filesystem::path filePath = std::filesystem::absolute(currentDir + "/" + args[1]);
        std::error_code ec;
        auto canonicalFile = std::filesystem::weakly_canonical(filePath, ec);
        if (ec || !isPathAllowed(canonicalFile, root)) {
            return "550 Permission denied.\r\n";
        }

        std::string result = DataTransfer::receiveFile(dataSocket, canonicalFile.string());
        if (result != "226 Transfer complete.\r\n")
            return result;
        return "";
    } catch (const std::exception& e) {
        return "550 Failed to store file (exception).\r\n";
    }
}

std::string CommandParser::handleDELE(const std::vector<std::string>& args, const std::string& currentDir, const std::string& rootDir) {
    if (args.size() < 2) {
        return "501 Syntax error in parameters.\r\n";
    }
    try {
        std::filesystem::path root = rootDir;
        std::filesystem::path filePath = std::filesystem::absolute(currentDir + "/" + args[1]);
        std::error_code ec;
        auto canonicalFile = std::filesystem::weakly_canonical(filePath, ec);
        if (ec || !std::filesystem::exists(canonicalFile) || !std::filesystem::is_regular_file(canonicalFile)) {
            return "550 File not found.\r\n";
        }
        if (!isPathAllowed(canonicalFile, root)) {
            return "550 Permission denied.\r\n";
        }

        if (std::filesystem::remove(canonicalFile)) {
            return "250 File deleted successfully.\r\n";
        } else {
            return "550 Failed to delete file.\r\n";
        }
    } catch (const std::exception& e) {
        return "550 Failed to delete file (exception).\r\n";
    }
}

std::string CommandParser::handleHELP(const std::vector<std::string>& args) {
    if (args.size() == 2) {
        std::string cmd = toUpperCase(args[1]);
        auto it = helpMessages.find(cmd);
        if (it != helpMessages.end()) {
            return "214 - " + it->second + "214 - HELP DONE\r\n";
        } else {
            return "503 Command not found. Please try HELP\r\n";
        }
    } else {
        std::string msg = "214 FTP-The following commands are recognized:\r\n";
        for (const auto& pair : helpMessages) {
            msg += "\t" + pair.first + "\n";
        }
        msg += "214 - HELP DONE\r\n";
        return msg;
    }
}

std::string CommandParser::handleCDUP(std::string& currentDir, const std::string& rootDir) {
    try {
        std::filesystem::path root = rootDir;
        std::filesystem::path parentPath = std::filesystem::absolute(currentDir).parent_path();
        std::error_code ec;
        auto canonicalParent = std::filesystem::weakly_canonical(parentPath, ec);

        if (ec || !std::filesystem::exists(canonicalParent) || !std::filesystem::is_directory(canonicalParent)) {
            return "550 can't change current directory\r\n";
        }
        if (!isPathAllowed(canonicalParent, root)) {
            return "550 Permission denied.\r\n";
        }
        currentDir = canonicalParent.string();
        return "200 Directory was changed successfully\r\n";
    } catch (const std::exception& e) {
        return "550 Failed to change to parent directory (exception).\r\n";
    }
}

CommandParser::PasvResult CommandParser::handlePASV(int clientSocket) {
    PasvResult result;
    try {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint16_t> dist(20000, 21000);
        result.port = dist(gen);
        struct sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);
        if (getsockname(clientSocket, (struct sockaddr*)&addr, &addrlen) == -1) {
            result.response = "425 Can't get server address.\r\n";
            result.port = 0;
            return result;
        }
        uint32_t ip = ntohl(addr.sin_addr.s_addr);
        uint8_t a = (ip >> 24) & 0xFF;
        uint8_t b = (ip >> 16) & 0xFF;
        uint8_t c = (ip >> 8) & 0xFF;
        uint8_t d = ip & 0xFF;
        uint8_t p1 = (result.port >> 8) & 0xFF;
        uint8_t p2 = result.port & 0xFF;

        char response[128];
        snprintf(response, sizeof(response),
            "227 Entering Passive Mode (%u,%u,%u,%u,%u,%u).\r\n", a, b, c, d, p1, p2);
        result.response = response;
        return result;
    } catch (const std::exception& e) {
        result.response = "425 Failed to enter passive mode (exception).\r\n";
        result.port = 0;
        return result;
    }
}

std::string CommandParser::handleTYPE(const std::vector<std::string>& args) {
    try {
        if (args.size() < 2)
            return "501 Syntax error in parameters or arguments.\r\n";
        std::string type = toUpperCase(args[1]);
        if (type == "I") {
            return "200 Switching to Binary mode.\r\n";
        } else if (type == "A") {
            return "200 Switching to ASCII mode.\r\n";
        } else {
            return "504 Command not implemented for that parameter.\r\n";
        }
    } catch (const std::exception& e) {
        return "451 Requested action aborted (exception). \r\n";
    }
}

std::string CommandParser::handlePORT(const std::vector<std::string>& args, ClientHandler& handler) {
    if (args.size() < 2)
        return "501 Syntax error in parameters or arguments.\r\n";

    int h[4], p[2];
    if (sscanf(args[1].c_str(), "%d,%d,%d,%d,%d,%d", &h[0], &h[1], &h[2], &h[3], &p[0], &p[1]) != 6)
        return "501 Syntax error in parameters or arguments.\r\n";

    char ip[32];
    snprintf(ip, sizeof(ip), "%d.%d.%d.%d", h[0], h[1], h[2], h[3]);
    uint16_t port = (p[0] << 8) | p[1];

    handler.setActiveMode(ip, port);

    return "200 PORT command successful.\r\n";
}