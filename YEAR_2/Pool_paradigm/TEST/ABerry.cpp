/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD10-fabien.fraixanet
** File description:
** ABerry
*/

#include "ABerry.hpp"

ABerry::ABerry(const std::string &name, unsigned int vitamins) : AFruit(name, vitamins)
{
    m_peeled = true;
}
