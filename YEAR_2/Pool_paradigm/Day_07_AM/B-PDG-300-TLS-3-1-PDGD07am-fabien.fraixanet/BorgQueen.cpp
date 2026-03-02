/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07am-fabien.fraixanet
** File description:
** BorgQueen
*/

#include <iostream>
#include "BorgQueen.hpp"

Borg::BorgQueen::BorgQueen()
{
    movePtr = &Borg::Ship::move;
    firePtr = &Borg::Ship::fire;
    destroyPtr = &Borg::Ship::fire;
}

Borg::BorgQueen::~BorgQueen()
{
}

bool Borg::BorgQueen::move(Borg::Ship *ship, Destination dest)
{
    return (ship->*movePtr)(dest);
}

void Borg::BorgQueen::fire(Borg::Ship *ship, Federation::Starfleet::Ship *target)
{
    (ship->*firePtr)(target);
}

void Borg::BorgQueen::destroy(Borg::Ship *ship, Federation::Ship *target)
{
    (ship->*destroyPtr)(target);
}
