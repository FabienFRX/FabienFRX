/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Map
*/

#ifndef MAP_HPP
#define MAP_HPP

#include <string>
#include <vector>

class Map {
public:
    explicit Map(const std::string &file);
    const std::vector<std::string>& getData() const;

private:
    std::vector<std::string> m_data;
    void loadMap(const std::string &file);
};

#endif /* !MAP_HPP_ */
