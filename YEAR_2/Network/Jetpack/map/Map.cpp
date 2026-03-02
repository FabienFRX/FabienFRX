/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Map
*/

#include <fstream>
#include <stdexcept>
#include "Map.hpp"

Map::Map(const std::string &file)
{
    loadMap(file);
}

const std::vector<std::string>& Map::getData() const
{
    return m_data;
}

void Map::loadMap(const std::string &file)
{
    std::ifstream fileStream(file);
    
    if (!fileStream.is_open()) {
        throw std::runtime_error("Failed to open map file: " + file);
    }
    
    std::string line;
    while (std::getline(fileStream, line)) {
        m_data.push_back(line);
    }
    
    fileStream.close();
    
    if (m_data.empty()) {
        throw std::runtime_error("Map file is empty: " + file);
    }
}
