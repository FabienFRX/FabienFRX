/*
** EPITECH PROJECT, 2025
** Day_09
** File description:
** Paladin
*/

#include "Paladin.hpp"
#include <iostream>

Paladin::Paladin(const std::string &name, int power) 
    : Peasant(name, power), Knight(name, power), Enchanter(name, power), Priest(name, power)
{
    std::cout << m_name << " fights for the light." << std::endl;
}

Paladin::~Paladin()
{
    std::cout << m_name << " is blessed." << std::endl;
}

int Paladin::attack()
{
    return Knight::attack();
}

int Paladin::special()
{
    return Enchanter::special();
}

void Paladin::rest()
{
    Priest::rest();
}
