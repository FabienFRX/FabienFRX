/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Fri, Apr, 2025                                                     *
 * Title           - B-NWP-400-TLS-4-1-jetpack-fabien.fraixanet                         *
 * Description     -                                                                    *
 *     Client                                                                           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "Client.hpp"

Client::Client(int fd, const std::string ip, int port) : _fd_client(fd), _ip(ip), _port_client(port) {}

Client::~Client() {
    if (_fd_client != -1) {
        close(_fd_client);
    }
}

int Client::get_fd() const {
    return _fd_client;
}

void Client::send_rep(const std::string response) {
    if (send(_fd_client, response.c_str(), response.size(), 0) == -1) {
        throw ClientError("Error: Failed to send response to client");
    }
}
