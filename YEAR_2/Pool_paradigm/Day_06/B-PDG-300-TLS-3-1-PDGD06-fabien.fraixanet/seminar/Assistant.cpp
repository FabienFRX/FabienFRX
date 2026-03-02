/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD06-fabien.fraixanet
** File description:
** Assistant
*/

#include <iostream>
#include <fstream>
#include <string>
#include "Assistant.hpp"

Assistant::Assistant(int ID) : m_ID(ID), m_isWorking(false)
{
    std::cout << "Assistant " << m_ID << ": 'morning everyone *sip coffee*" << std::endl;
}

Assistant::~Assistant()
{
    std::cout << "Assistant " << m_ID << ": see you tomorrow at 9.00 *sip coffee*" << std::endl;
}

void Assistant::giveDrink(Student* student, const std::string& drink)
{
    std::cout << "Assistant " << m_ID << ": drink this, " << student->getName() << " *sip coffee*" << std::endl;
    student->drink(drink);
}

std::string Assistant::readDrink(const std::string& studentName)
{
    std::string fileName = studentName + ".drink";
    std::ifstream file(fileName);
    std::string drink;

    if (!file.is_open()) {
        return "";
    }
    std::getline(file, drink);
    file.close();
    std::remove(fileName.c_str());
    std::cout << "Assistant " << m_ID << ": " << studentName << " needs a " << drink << " *sip coffee*" << std::endl;
    return drink;
}

void Assistant::helpStudent(Student* student)
{
    std::string drink = readDrink(student->getName());

    if (drink.empty()) {
        std::cout << "Assistant " << m_ID << ": " << student->getName() << " seems fine *sip coffee*" << std::endl;
    } else {
        giveDrink(student, drink);
    }
}

void Assistant::timeCheck()
{
    if (m_isWorking) {
        std::cout << "Assistant " << m_ID << ": Enough teaching for today *sip coffee*" << std::endl;
    } else {
        std::cout << "Assistant " << m_ID << ": Time to teach some serious business *sip coffee*" << std::endl;
    }
    m_isWorking = !m_isWorking;
}
