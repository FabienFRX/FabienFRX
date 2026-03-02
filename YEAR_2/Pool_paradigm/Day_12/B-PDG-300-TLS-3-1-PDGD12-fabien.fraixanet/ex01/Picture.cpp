/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD12-fabien.fraixanet
** File description:
** Picture
*/

#include <iostream>
#include <fstream>
#include "Picture.hpp"

Picture::Picture() : data("")
{
}

Picture::Picture(const std::string &file)
{
    if (!file.empty()) {
        getPictureFromFile(file);
    } else
        data.clear();
}

Picture::Picture(const Picture &other) : data(other.data)
{
}

Picture &Picture::operator=(const Picture &other)
{
    if (this != &other) {
        data = other.data;
    }
    return *this;
}

Picture::~Picture()
{
}

bool Picture::getPictureFromFile(const std::string &file)
{
    std::ifstream infile(file);
    std::string line;

    if (!infile.is_open()) {
        data = "ERROR";
        return false;
    }
    data.clear();
    while (std::getline(infile, line)) {
        data += line + "\n";
    }
    return true;
}
