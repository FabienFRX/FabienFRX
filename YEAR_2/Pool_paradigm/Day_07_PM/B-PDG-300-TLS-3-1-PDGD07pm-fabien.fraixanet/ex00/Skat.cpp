/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07pm-fabien.fraixanet
** File description:
** Skat
*/

#include "Skat.hpp"

Skat::Skat(const std::string &name, int stimPaks) : m_name(name), m_stimPaks(stimPaks)
{
}

Skat::~Skat()
{
}

int &Skat::stimPaks()
{
    return m_stimPaks;
}

const std::string &Skat::name()
{
    return m_name;
}

void Skat::shareStimPaks(int number, int &stock)
{
    if (number > m_stimPaks) {
        std::cout << "Don't be greedy" << std::endl;
    } else {
        m_stimPaks -= number;
        stock += number;
        std::cout << "Keep the change." << std::endl;
    }
}

void Skat::addStimPaks(unsigned int number)
{
    if (number == 0)
        std::cout << "Hey boya, did you forget something?" << std::endl;
    else
        m_stimPaks += number;
}

void Skat::useStimPaks()
{
    if (m_stimPaks > 0) {
        std::cout << "Time to kick some ass and chew bubble gum." << std::endl;
        m_stimPaks--;
    } else {
        std::cout << "Mediiiiiic" << std::endl;
    }
}

void Skat::status() const
{
    std::cout << "Soldier " << m_name << " reporting " << m_stimPaks
        << " stimpaks remaining sir!" << std::endl;
}
