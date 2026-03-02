/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD10-fabien.fraixanet
** File description:
** FruitBox
*/

#include "FruitBox.hpp"

FruitBox::FruitBox(unsigned int size) : m_size(size)
{
}

FruitBox::~FruitBox()
{
    for (auto fruit : m_fruits) {
        delete fruit;
    }
}

unsigned int FruitBox::getSize() const
{
    return m_size;
}

unsigned int FruitBox::nbFruits() const
{
    return m_fruits.size();
}

bool FruitBox::pushFruit(IFruit* fruit)
{
    if (nbFruits() >= m_size) return false;

    for (auto& f : m_fruits) {
        if (f == fruit) return false;
    }

    m_fruits.push_back(fruit);
    return true;
}

IFruit* FruitBox::popFruit()
{
    if (m_fruits.empty()) return nullptr;

    IFruit* fruit = m_fruits.front();
    m_fruits.erase(m_fruits.begin());
    return fruit;
}

std::string FruitBox::toString() const
{
    std::string result = "[";
    for (size_t i = 0; i < m_fruits.size(); ++i) {
        result += "[name: \"" + m_fruits[i]->getName() + "\", ";
        result += "vitamins: " + std::to_string(m_fruits[i]->getVitamins()) + ", ";
        result += "peeled: " + std::string(m_fruits[i]->isPeeled() ? "true" : "false") + "]";
        if (i < m_fruits.size() - 1) {
            result += ", ";
        }
    }
    result += "]";
    return result;
}

std::ostream& operator<<(std::ostream& os, const FruitBox& box)
{
    os << box.toString();
    return os;
}
