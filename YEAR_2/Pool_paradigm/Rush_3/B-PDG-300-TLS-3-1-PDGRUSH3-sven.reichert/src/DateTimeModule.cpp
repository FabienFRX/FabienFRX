/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sun, Feb, 2025                                                     *
 * Title           - B-PDG-300-TLS-3-1-PDGRUSH3-sven.reichert [WSL: Debian]             *
 * Description     -                                                                    *
 *     DateTimeModule                                                                   *
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

#include "../include/DateTimeModule.hpp"

Krell::DateTimeModule::DateTimeModule()
{
    this->update();
}

void Krell::DateTimeModule::update()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    this->_data.clear();
    this->_data.push_back(std::to_string(1900 + ltm->tm_year));
    this->_data.push_back(std::to_string(1 + ltm->tm_mon));
    this->_data.push_back(std::to_string(ltm->tm_mday));
    this->_data.push_back(std::to_string(ltm->tm_hour));
    this->_data.push_back(std::to_string(ltm->tm_min));
    this->_data.push_back(std::to_string(ltm->tm_sec));
}

void Krell::DateTimeModule::render(IDisplay &display)
{
    std::string date = "Date: " + this->_data[2] + "/" + this->_data[1] + "/" + this->_data[0];
    std::string time = "Time: " + this->_data[3] + ":" + this->_data[4] + ":" + this->_data[5];
    
    display.draw(date);
    display.draw(time);
}

std::string Krell::DateTimeModule::getData() const
{
    return this->_data[0] + "/" + this->_data[1] + "/" + this->_data[2] + " " + this->_data[3] + ":" + this->_data[4] + ":" + this->_data[5];
}