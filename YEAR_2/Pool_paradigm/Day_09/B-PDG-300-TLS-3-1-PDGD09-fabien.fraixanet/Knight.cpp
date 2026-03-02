/*
** EPITECH PROJECT, 2025
** Day_09
** File description:
** Knight
*/

#include <iostream>
#include "Knight.hpp"

Knight::Knight(const std::string &name, int power) : Peasant(name, power)
{
    std::cout << m_name << " vows to protect the kingdom." << std::endl;
}

Knight::~Knight()
{
    std::cout << m_name << " takes off his armor." << std::endl;
}

int Knight::attack()
{
    if (m_hp <= 0) {
        std::cout << m_name << " is out of combat." << std::endl;
        return 0;
    }
    if (m_power < 10) {
        std::cout << m_name << " is out of power." << std::endl;
        return 0;
    }
    m_power -= 10;
    std::cout << m_name << " strikes with his sword." << std::endl;
    return 20;
}

int Knight::special()
{
    if (m_hp <= 0) {
        std::cout << m_name << " is out of combat." << std::endl;
        return 0;
    }
    if (m_power < 30) {
        std::cout << m_name << " is out of power." << std::endl;
        return 0;
    }
    m_power -= 30;
    std::cout << m_name << " impales his enemy." << std::endl;
    return 50;
}

void Knight::rest()
{
    if (m_hp == 0) {
        std::cout << m_name << " is out of combat." << std::endl;
        return;
    }
    m_power += 50;
    capValues();
    std::cout << m_name << " eats." << std::endl;
}
