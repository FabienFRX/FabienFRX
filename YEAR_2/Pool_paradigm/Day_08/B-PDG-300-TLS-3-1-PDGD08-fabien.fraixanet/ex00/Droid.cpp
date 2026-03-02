/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD08-fabien.fraixanet
** File description:
** Droid
*/

#include <iostream>
#include <string>
#include "Droid.hpp"

Droid::Droid(const std::string& serial) : m_Id(serial), m_Energy(50), m_Attack(25),
    m_Toughness(15), m_Status(new std::string("Standing by"))
{
    std::cout << "Droid '" << m_Id << "' Activated" << std::endl;
}

Droid::Droid(const Droid& other) : m_Id(other.m_Id), m_Energy(other.m_Energy), m_Attack(other.m_Attack),
    m_Toughness(other.m_Toughness), m_Status(new std::string(*other.m_Status))
{
    std::cout << "Droid '" << m_Id << "' Activated, Memory Dumped" << std::endl;
}

Droid::~Droid()
{
    std::cout << "Droid '" << m_Id << "' Destroyed" << std::endl;
    delete m_Status;
}

const std::string& Droid::getId() const
{
    return m_Id;
}

size_t Droid::getEnergy() const
{
    return m_Energy;
}

size_t Droid::getAttack() const
{
    return m_Attack;
}

size_t Droid::getToughness() const
{
    return m_Toughness;
}

const std::string& Droid::getStatus() const
{
    return *m_Status;
}

void Droid::setId(const std::string& id)
{
    m_Id = id;
}

void Droid::setEnergy(size_t energy)
{
    if (energy > 100) {
        m_Energy = 100;
    } else {
        m_Energy = energy;
    }
}

void Droid::setStatus(std::string* status)
{
    if (m_Status != status) {
        delete m_Status;
        m_Status = status;
    }
}

Droid& Droid::operator=(const Droid& other)
{
    if (this != &other) {
        m_Id = other.m_Id;
        m_Energy = other.m_Energy;
        *m_Status = *other.m_Status;
    }
    return *this;
}

bool Droid::operator==(const Droid& other) const
{
    return m_Id == other.m_Id &&
           m_Energy == other.m_Energy &&
           m_Attack == other.m_Attack &&
           m_Toughness == other.m_Toughness &&
           *m_Status == *other.m_Status;
}

bool Droid::operator!=(const Droid& other) const
{
    return !(*this == other);
}

Droid& Droid::operator<<(size_t& energy)
{
    size_t transfer = 0;

    if (m_Energy + energy > 100) {
        transfer = 100 - m_Energy;
    } else {
        transfer = energy;
    }
    m_Energy += transfer;
    energy -= transfer;
    return *this;
}

std::ostream& Droid::print(std::ostream& os) const
{
    os << "Droid '" << m_Id << "', " << *m_Status << ", " << m_Energy;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Droid& droid)
{
    return droid.print(os);
}
