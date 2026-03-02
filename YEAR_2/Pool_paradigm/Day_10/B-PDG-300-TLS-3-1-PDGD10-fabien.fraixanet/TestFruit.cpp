/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD10-fabien.fraixanet
** File description:
** TestFruit
*/

#include "TestFruit.hpp"
#include <iostream>

TestFruit::TestFruit(const std::string& name)
    : m_name(name), m_vitamins(0), m_isPeeled(false)
{
    std::cout << m_name << " lives." << std::endl;
}

TestFruit::~TestFruit()
{
    std::cout << m_name << " dies." << std::endl;
}

std::string TestFruit::getName() const
{
    return m_name;
}

unsigned int TestFruit::getVitamins() const
{
    return m_vitamins;
}

bool TestFruit::isPeeled() const
{
    return m_isPeeled;
}

void TestFruit::peel()
{
    m_isPeeled = true;
}
