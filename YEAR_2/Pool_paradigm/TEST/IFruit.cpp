/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD10-fabien.fraixanet
** File description:
** IFruit
*/

#include <iostream>
#include <string>
#include "IFruit.hpp"

std::ostream &operator<<(std::ostream &os, const IFruit &fruit)
{
    os << "[name: \"" << fruit.getName()
       << "\", vitamins: " << fruit.getVitamins()
       << ", peeled: " << (fruit.isPeeled() ? "true" : "false") << "]";
    return os;
}
