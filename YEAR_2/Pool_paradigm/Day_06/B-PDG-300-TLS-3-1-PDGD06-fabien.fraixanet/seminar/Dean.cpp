/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD06-fabien.fraixanet
** File description:
** Dean
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "Dean.hpp"

Dean::Dean(const std::string& name) : m_name(name), m_isWorking(false)
{
    std::cout << "Dean " << m_name << ": I'm Dean " << m_name << "! How do you do, fellow kids?" << std::endl;
}

Dean::~Dean()
{
    std::cout << "Dean " << m_name << ": Time to go home." << std::endl;
}

void Dean::teachStudent(Student* student, const std::string& lesson)
{
    std::string drinks[] = {"Cristaline", "Monster", "Evian", "Red Bull", "Sierra Springs"};
    std::string filename = student->getName() + ".drink";
    std::string selectedDrink;
    std::ofstream file(filename, std::ios::app);

    if (student->learn(lesson))
        return;
    std::cout << "Dean " << m_name << ": All work and no play makes " << student->getName() << " a dull student." << std::endl;
    selectedDrink = drinks[std::rand() % 5];
    if (file.is_open()) {
        file << selectedDrink;
        file.close();
    }
}

void Dean::timeCheck()
{
    if (m_isWorking)
        std::cout << "Dean " << m_name << ": Don't forget to close the windows when you leave." << std::endl;
    else
        std::cout << "Dean " << m_name << ": Where is everyone?" << std::endl;
    m_isWorking = !m_isWorking;
}
