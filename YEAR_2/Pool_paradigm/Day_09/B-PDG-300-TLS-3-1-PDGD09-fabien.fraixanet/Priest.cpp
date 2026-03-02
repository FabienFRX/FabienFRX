/*
** EPITECH PROJECT, 2025
** Day_09
** File description:
** Priest
*/

#include "Priest.hpp"
#include <iostream>

Priest::Priest(const std::string &name, int power) : Peasant(name, power), Enchanter(name, power)
{
    std::cout << m_name << " enters in the order." << std::endl;
}

Priest::~Priest()
{
    std::cout << m_name << " finds peace." << std::endl;
}

void Priest::rest()
{
    if (m_hp <= 0) {
        std::cout << m_name << " is out of combat." << std::endl;
        return;
    }
    m_power += 100;
    m_hp += 100;
    capValues();
    std::cout << m_name << " prays." << std::endl;
}
