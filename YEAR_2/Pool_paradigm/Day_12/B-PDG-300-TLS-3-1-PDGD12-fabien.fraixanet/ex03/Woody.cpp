/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD12-fabien.fraixanet
** File description:
** Woody
*/

#include "Woody.hpp"

Woody::Woody(const std::string &name, const std::string &file) : Toy(WOODY, name, file)
{
}

void Woody::speak(const std::string &statement)
{
    std::cout << "WOODY: " << m_name << " \"" << statement << "\"" << std::endl;
}
