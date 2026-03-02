/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07am-fabien.fraixanet
** File description:
** Admiral
*/

#include "Admiral.hpp"
#include <iostream>

Federation::Starfleet::Admiral::Admiral(const std::string &name) : _name(name)
{
    firePtr = &Federation::Starfleet::Ship::fire;
    movePtr = &Federation::Starfleet::Ship::move;

    std::cout << "Admiral " << _name << " ready for action." << std::endl;
}

Federation::Starfleet::Admiral::~Admiral()
{
}

void Federation::Starfleet::Admiral::Admiral::fire(Federation::Starfleet::Ship *ship, Borg::Ship *target)
{
    std::cout << "On order from Admiral " << _name << ":" << std::endl;
    (ship->*firePtr)(target);
}

bool Federation::Starfleet::Admiral::Admiral::move(Federation::Starfleet::Ship *ship, Destination dest)
{
    return (ship->*movePtr)(dest);
}
