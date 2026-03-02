/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Fri, Apr, 2025                                                     *
 * Title           - B-NWP-400-TLS-4-1-jetpack-fabien.fraixanet                         *
 * Description     -                                                                    *
 *     Server                                                                           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░             *
 *         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓             *
 *         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include <vector>
#include "Server.hpp"
#include "Protocol.hpp"

static int create_principal_sock(int port)
{
    struct sockaddr_in sockaddr;
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
        throw ServerError("Error: Failed to create socket");

    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    sockaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) != 0) {
        close(sock);
        throw ServerError("Error: Failed to bind socket");
    }
    if (listen(sock, 100) == -1) {
        close(sock);
        throw ServerError("Error: Failed to listen on socket");
    }
    return sock;
}

Server::Server(int port, const std::vector<std::string> mapContent, bool debug)
    : _fd_server(-1), _port_server(port), _map(mapContent), _debug(debug)
{
    _fd_server = create_principal_sock(port);
    _game = std::make_unique<Game>(mapContent);
    _lastUpdateTime = std::chrono::steady_clock::now();
}

void Server::debug_log(const std::string &message)
{
    if (_debug) {
        std::cout << "[DEBUG] " << message << std::endl;
    }
}

Server::~Server()
{
    if (_fd_server != -1) {
        close(_fd_server);
    }
    for (auto client : _clients) {
        delete client;
    }
    _clients.clear();
}

int Server::get_fd() const
{
    return (_fd_server);
}

int Server::get_port() const
{
    return (_port_server);
}

static void add_new_fd(std::vector<struct pollfd> *fds, int sock)
{
    struct pollfd new_fds;
    new_fds.fd = sock;
    new_fds.events = POLLIN;
    fds->push_back(new_fds);
}

void Server::new_connection(std::vector<struct pollfd> *fds)
{
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = accept(get_fd(), (struct sockaddr *)&client_addr, &addr_len);

    if (client_fd == -1) {
        throw ServerError("Error: Failed to accept client connection");
    }

    try {
        Client *new_client = new Client(client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        _clients.push_back(new_client);
        add_new_fd(fds, client_fd);

        Packet hello;
        hello.type = PacketType::HELLO;
        hello.payload = stringToBytes("connected");

        std::vector<uint8_t> data = hello.serialize();
        if (send(client_fd, data.data(), data.size(), 0) == -1) {
            throw ServerError("Error: Failed to send HELLO packet to client");
        }
    } catch (...) {
        close(client_fd);
        throw;
    }
}

void Server::launch_server()
{
    std::vector<struct pollfd> fds;
    add_new_fd(&fds, _fd_server);

    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - _lastUpdateTime).count();
        _lastUpdateTime = currentTime;

        _game->update(deltaTime);

        if (!_game->isGameStarted() && _game->canStartGame()) {
            _game->startGame();
        }

        if (poll(fds.data(), fds.size(), 10) < 0) {
            throw ServerError("Error: Polling failed");
        }

        for (size_t i = 0; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == _fd_server) {
                    new_connection(&fds);
                } else {
                    handle_client_data(fds[i].fd);
                }
            }
        }
    }
}

void Server::handle_client_data(int client_fd)
{
    char buf[1024];
    int bytesRead = recv(client_fd, buf, sizeof(buf), 0);

    if (bytesRead <= 0) {
        auto it = std::find_if(_clients.begin(), _clients.end(),
                               [client_fd](const Client *c) { return c->get_fd() == client_fd; });

        if (it != _clients.end()) {
            _game->removePlayer(*it);
            delete *it;
            _clients.erase(it);
        }

        auto pollIt = std::remove_if(fds.begin(), fds.end(),
                                     [client_fd](const struct pollfd &pfd) { return pfd.fd == client_fd; });
        fds.erase(pollIt, fds.end());

        return;
    }

    std::vector<uint8_t> raw(buf, buf + bytesRead);
    debug_log("Received " + std::to_string(bytesRead) + " bytes");

    try {
        Packet pkt = Packet::deserialize(raw);
        debug_log("Packet type: " + std::to_string(static_cast<int>(pkt.type)));

        Client *client = nullptr;
        for (auto c : _clients) {
            if (c->get_fd() == client_fd) {
                client = c;
                break;
            }
        }

        if (!client) {
            debug_log("Client not found for fd: " + std::to_string(client_fd));
            return;
        }

        switch (pkt.type) {
        case PacketType::PLAYER_MOVE: {
            std::string move = bytesToString(pkt.payload);
            debug_log("Player move: " + move);
            _game->handlePlayerInput(client, move);
            break;
        }
        default:
            debug_log("Unknown or unhandled packet type");
        }
    } catch (const std::exception &e) {
        debug_log("Invalid packet: " + std::string(e.what()));
    }
}
