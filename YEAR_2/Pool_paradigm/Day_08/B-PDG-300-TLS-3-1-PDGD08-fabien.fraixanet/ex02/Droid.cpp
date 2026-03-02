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
    m_Toughness(15), m_Status(new std::string("Standing by")), m_BattleData(new DroidMemory())
{
    std::cout << "Droid '" << m_Id << "' Activated" << std::endl;
}

Droid::Droid(const Droid& other) : m_Id(other.m_Id), m_Energy(50), m_Attack(other.m_Attack),
    m_Toughness(other.m_Toughness), m_Status(new std::string(*other.m_Status)),
    m_BattleData(new DroidMemory(*other.m_BattleData))
{
    std::cout << "Droid '" << m_Id << "' Activated, Memory Dumped" << std::endl;
}

Droid::~Droid()
{
    delete m_Status;
    delete m_BattleData;
    std::cout << "Droid '" << m_Id << "' Destroyed" << std::endl;
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

DroidMemory* Droid::getBattleData() const
{
    return m_BattleData;
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

void Droid::setBattleData(DroidMemory* battleData)
{
    if (m_BattleData != battleData) {
        if (m_BattleData != nullptr) {
            delete m_BattleData;
        }
        m_BattleData = battleData;
    }
}

//Droid& Droid::operator=(const Droid& other)
//{
//    if (this != &other) {
//        m_Id = other.m_Id;
//        *m_Status = *other.m_Status;
//        if (m_BattleData != nullptr)
//            delete m_BattleData;
//        m_BattleData = new DroidMemory(*other.m_BattleData);
//        m_Energy = 50;
//    }
//    return *this;
//}

bool Droid::operator==(const Droid& other) const
{
    return m_Id == other.m_Id &&
        *m_Status == *other.m_Status;
}

bool Droid::operator!=(const Droid& other) const
{
    return !(*this == other);
}

Droid& Droid::operator<<(size_t &energy)
{
    size_t transfer = std::min(energy, 100 - m_Energy);

    m_Energy += transfer;
    energy -= transfer;
    return *this;
}

bool Droid::operator()(const std::string* task, size_t requiredExp)
{
    if (m_Energy < 10) {
        m_Energy = 0;
        *m_Status = "Battery Low";
        return false;
    }
    m_Energy -= 10;

    if (m_BattleData->getExp() >= requiredExp) {
        m_BattleData->setExp(m_BattleData->getExp() + requiredExp / 2);
        *m_Status = *task + " - Completed!";
        return true;
    } else {
        m_BattleData->setExp(m_BattleData->getExp() + requiredExp);
        *m_Status = *task + " - Failed!";
        return false;
    }
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