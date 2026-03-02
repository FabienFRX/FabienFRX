/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD06-fabien.fraixanet
** File description:
** Student
*/

#include <iostream>
#include <string>
#include "Student.hpp"

Student::Student(const std::string& name) : m_name(name), m_energy(100)
{
    std::cout << "Student " << m_name << ": I'm ready to learn C++." << std::endl;
}

Student::~Student()
{
    std::cout << "Student " << m_name << ": Wow, much learning today, very smart, such C++." << std::endl;
}

bool Student::learn(const std::string& text)
{
    if (m_energy < 42) {
        std::string modifiedText = text;
        size_t pos = 0;

    while (true) {
        pos = modifiedText.find("C++", pos);
        if (pos == std::string::npos) {
            break;
        }
        modifiedText.replace(pos, 3, "shit");
        pos += 4;
    }
        std::cout << "Student " << m_name << ": " << modifiedText << std::endl;
        return false;
    }
    m_energy -= 42;
    std::cout << "Student " << m_name << ": " << text << std::endl;
    return true;
}

void Student::drink(const std::string& drink)
{
    if (drink == "Red Bull") {
        std::cout << "Student " << m_name << ": Red Bull gives you wings!" << std::endl;
        m_energy = std::min(100, m_energy + 32);
    } else if (drink == "Monster") {
        std::cout << "Student " << m_name << ": Unleash The Beast!" << std::endl;
        m_energy = std::min(100, m_energy + 64);
    } else {
        std::cout << "Student " << m_name << ": ah, yes... enslaved moisture." << std::endl;
        m_energy = std::min(100, m_energy + 1);
    }
}

std::string Student::getName() const
{
    return m_name;
}
