/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD10-fabien.fraixanet
** File description:
** AFruit
*/

#include <iostream>
#include "AFruit.hpp"

AFruit::AFruit(const std::string& name, unsigned int vitamins) : m_name(name), m_vitamins(vitamins), m_peeled(false)
{
}

AFruit::~AFruit()
{
}

unsigned int AFruit::getVitamins() const
{
    if (m_peeled)
        return m_vitamins;
    return 0;
}

std::string AFruit::getName() const
{
    return m_name;
}

bool AFruit::isPeeled() const
{
    return m_peeled;
}

void AFruit::peel()
{
    m_peeled = true;
}
