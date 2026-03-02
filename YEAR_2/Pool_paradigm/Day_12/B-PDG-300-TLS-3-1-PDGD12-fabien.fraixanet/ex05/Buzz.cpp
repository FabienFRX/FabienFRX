/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD12-fabien.fraixanet
** File description:
** Buzz
*/

#include "Buzz.hpp"

Buzz::Buzz(const std::string &name, const std::string &file) : Toy(BUZZ, name, file)
{
}

void Buzz::speak(const std::string &statement)
{
    std::cout << "BUZZ: " << m_name << " \"" << statement << "\"" << std::endl;
}

bool Buzz::speak_es(const std::string &statement)
{
    std::cout << "BUZZ: " << m_name << " senorita \"" << statement << "\" senorita" << std::endl;
    return true;
}
