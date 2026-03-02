/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07pm-fabien.fraixanet
** File description:
** Parts
*/

/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07pm-fabien.fraixanet
** File description:
** Parts
*/

#include "Parts.hpp"

Arms::Arms(std::string serial, bool functional) : m_serial(serial), m_functional(functional)
{
}

bool Arms::isFunctional() const
{
    return m_functional;
}

std::string Arms::serial() const
{
    return m_serial;
}

void Arms::informations() const
{
    std::cout << "\t[Parts] Arms " << m_serial << " status : ";
    if (m_functional) {
        std::cout << "OK" << std::endl;
    } else {
        std::cout << "KO" << std::endl;
    }
}

Legs::Legs(std::string serial, bool functional) : m_serial(serial), m_functional(functional)
{
}

bool Legs::isFunctional() const
{
    return m_functional;
}

std::string Legs::serial() const
{
    return m_serial;
}

void Legs::informations() const
{
    std::cout << "\t[Parts] Legs " << m_serial << " status : ";
    if (m_functional) {
        std::cout << "OK" << std::endl;
    } else {
        std::cout << "KO" << std::endl;
    }
}

Head::Head(std::string serial, bool functional) : m_serial(serial), m_functional(functional)
{
}

bool Head::isFunctional() const
{
    return m_functional;
}

std::string Head::serial() const
{
    return m_serial;
}

void Head::informations() const
{
    std::cout << "\t[Parts] Head " << m_serial << " status : ";
    if (m_functional) {
        std::cout << "OK" << std::endl;
    } else {
        std::cout << "KO" << std::endl;
    }
}
