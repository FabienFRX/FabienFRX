/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD08-fabien.fraixanet
** File description:
** DroidMemory
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include "DroidMemory.hpp"

DroidMemory::DroidMemory() : m_Fingerprint(std::rand()), m_Exp(0)
{
}

DroidMemory::DroidMemory(const DroidMemory& other)
    : m_Fingerprint(other.m_Fingerprint), m_Exp(other.m_Exp)
{
}

DroidMemory::~DroidMemory()
{
}

size_t DroidMemory::getFingerprint() const
{
    return m_Fingerprint;
}

void DroidMemory::setFingerprint(size_t fingerprint)
{
    m_Fingerprint = fingerprint;
}

size_t DroidMemory::getExp() const
{
    return m_Exp;
}

void DroidMemory::setExp(size_t exp)
{
    m_Exp = exp;
}


DroidMemory& DroidMemory::operator=(const DroidMemory& other)
{
    if (this != &other) {
        m_Fingerprint = other.m_Fingerprint;
        m_Exp = other.m_Exp;
    }
    return *this;
}

DroidMemory& DroidMemory::operator<<(const DroidMemory& other)
{
    m_Exp += other.m_Exp;
    m_Fingerprint ^= other.m_Fingerprint;
    return *this;
}

DroidMemory& DroidMemory::operator>>(DroidMemory& other)
{
    other.m_Exp += m_Exp;
    other.m_Fingerprint ^= m_Fingerprint;
    return *this;
}

DroidMemory& DroidMemory::operator+=(const DroidMemory& other)
{
    return *this << other;
}

DroidMemory& DroidMemory::operator+=(size_t exp)
{
    m_Exp += exp;
    m_Fingerprint ^= exp;
    return *this;
}

DroidMemory DroidMemory::operator+(const DroidMemory& other) const
{
    DroidMemory result = *this;
    result += other;
    return result;
}

DroidMemory DroidMemory::operator+(size_t exp) const
{
    DroidMemory result = *this;
    result += exp;
    return result;
}

std::ostream& DroidMemory::print(std::ostream& os) const
{
    os << "DroidMemory '" << m_Fingerprint << "', " << m_Exp;
    return os;
}

std::ostream& operator<<(std::ostream& os, const DroidMemory& memory)
{
    return memory.print(os);
}
