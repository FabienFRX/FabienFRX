/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07pm-fabien.fraixanet
** File description:
** KoalaBot
*/

#include "KoalaBot.hpp"

KoalaBot::KoalaBot(std::string serial) : m_serial(serial), m_arms(), m_legs(), m_head()
{
}

bool KoalaBot::status() const
{
    return m_arms.isFunctional() && m_legs.isFunctional() && m_head.isFunctional();
}

void KoalaBot::informations() const
{
    std::cout << "[KoalaBot] " << m_serial << std::endl;
    m_arms.informations();
    m_legs.informations();
    m_head.informations();
}

void KoalaBot::setParts(const Arms &arms)
{
    m_arms = arms;
}

void KoalaBot::setParts(const Legs &legs)
{
    m_legs = legs;
}

void KoalaBot::setParts(const Head &head)
{
    m_head = head;
}

void KoalaBot::swapParts(Arms &arms)
{
    std::swap(m_arms, arms);
}

void KoalaBot::swapParts(Legs &legs)
{
    std::swap(m_legs, legs);
}

void KoalaBot::swapParts(Head &head)
{
    std::swap(m_head, head);
}
