/*
** EPITECH PROJECT, 2024
** B-YEP
** File description:
** Errors.hpp
*/

#ifndef ERROR_HPP
#define ERROR_HPP
    #pragma once
    #include <string>
    #include <iostream>
    #include "Map.hpp"
    #include "Client.hpp"

    bool checkError(int argc, char** argv, Map& map, Client& client, int& port, std::string& host);

#endif
