/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Client
*/

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "TotalIncludes.hpp"

class Client {
    public:
        Client(int fd, const std::string ip, int port);
        ~Client();

        void send_rep(const std::string response);

        int get_fd() const;

    private:
        int _fd_client;
        std::string _ip;
        int _port_client;
};

#endif /* !CLIENT_HPP_ */
