/*
** EPITECH PROJECT, 2025
** Day_09
** File description:
** Enchanter
*/

#include <iostream>
#include "Enchanter.hpp"

Enchanter::Enchanter(const std::string &name, int power) : Peasant(name, power)
{
    std::cout << m_name << " learns magic from his spellbook." << std::endl;
}

Enchanter::~Enchanter()
{
    std::cout << m_name << " closes his spellbook." << std::endl;
}

int Enchanter::attack()
{
    if (m_hp <= 0) {
        std::cout << m_name << " is out of combat." << std::endl;
        return 0;
    }
    std::cout << m_name << " doesn't know how to fight." << std::endl;
    return 0;
}

int Enchanter::special()
{
    if (m_hp <= 0) {
        std::cout << m_name << " is out of combat." << std::endl;
        return 0;
    }
    if (m_power < 50) {
        std::cout << m_name << " is out of power." << std::endl;
        return 0;
    }
    m_power -= 50;
    std::cout << m_name << " casts a fireball." << std::endl;
    return 99;
}

void Enchanter::rest()
{
    if (m_hp <= 0) {
        std::cout << m_name << " is out of combat." << std::endl;
        return;
    }
    m_power += 100;
    capValues();
    std::cout << m_name << " meditates." << std::endl;
}
