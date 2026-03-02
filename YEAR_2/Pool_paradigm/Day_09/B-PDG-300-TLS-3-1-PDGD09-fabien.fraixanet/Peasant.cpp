/*
** EPITECH PROJECT, 2025
** Day_09
** File description:
** Peasant
*/

#include "Peasant.hpp"

Peasant::Peasant(const std::string &name, int power) : m_name(name), m_power(power), m_hp(100)
{
    capValues();
    std::cout << m_name << " goes for an adventure." << std::endl;
}

Peasant::~Peasant()
{
    std::cout << m_name << " is back to his crops." << std::endl;
}

const std::string &Peasant::getName() const
{
    return m_name;
}

int Peasant::getPower() const
{
    return m_power;
}

int Peasant::getHp() const
{
    return m_hp;
}

int Peasant::attack()
{
    if (m_hp == 0) {
        std::cout << m_name << " is out of combat." << std::endl;
        return 0;
    }
    if (m_power < 10) {
        std::cout << m_name << " is out of power." << std::endl;
        return 0;
    }
    m_power -= 10;
    std::cout << m_name << " tosses a stone." << std::endl;
    return 5;
}

int Peasant::special()
{
    if (m_hp == 0) {
        std::cout << m_name << " is out of combat." << std::endl;
        return 0;
    }
    std::cout << m_name << " doesn't know any special move." << std::endl;
    return 0;
}

void Peasant::rest()
{
    if (m_hp == 0) {
        std::cout << m_name << " is out of combat." << std::endl;
        return;
    }
    m_power += 30;
    capValues();
    std::cout << m_name << " takes a nap." << std::endl;
}

void Peasant::damage(int damage)
{
    if (m_hp <= 0) {
        std::cout << m_name << " is out of combat." << std::endl;
        return;
    }
    if (m_hp - damage <= 0) {
        m_hp = 0;
        std::cout << m_name << " is out of combat." << std::endl;
    } else {
        m_hp -= damage;
        std::cout << m_name << " takes " << damage << " damage." << std::endl;
    }
}

void Peasant::capValues()
{
    if (m_power > 100) m_power = 100;
    if (m_power < 0) m_power = 0;
    if (m_hp > 100) m_hp = 100;
    if (m_hp < 0) m_hp = 0;
}
