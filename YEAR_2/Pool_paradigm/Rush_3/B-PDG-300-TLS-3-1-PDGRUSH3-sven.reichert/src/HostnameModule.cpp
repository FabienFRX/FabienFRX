/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sun, Feb, 2025                                                     *
 * Title           - B-PDG-300-TLS-3-1-PDGRUSH3-sven.reichert [WSL: Debian]             *
 * Description     -                                                                    *
 *     HostnameModule                                                                   *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "../include/HostnameModule.hpp"

Krell::HostnameModule::HostnameModule()
{
    this->update();
}

void Krell::HostnameModule::update()
{
    char hostname[256];
    char username[256];

    gethostname(hostname, 256);
    getlogin_r(username, 256);

    this->hostname = hostname;
    this->username = username;
}

void Krell::HostnameModule::render(IDisplay &display)
{
    display.draw("Hostname: " + this->hostname);
    display.draw("Username: " + this->username);
}

std::string Krell::HostnameModule::getData() const
{
    return this->hostname + " " + this->username;
}