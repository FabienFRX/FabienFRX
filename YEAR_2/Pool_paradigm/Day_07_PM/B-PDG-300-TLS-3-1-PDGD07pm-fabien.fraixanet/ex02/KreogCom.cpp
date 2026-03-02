/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07pm-fabien.fraixanet
** File description:
** KreogCom
*/

#include "KreogCom.hpp"

KreogCom::KreogCom(int x, int y, int serial) : m_serial(serial), m_x(x), m_y(y), m_next(nullptr)
{
    std::cout << "KreogCom " << m_serial << " initialized" << std::endl;
}

KreogCom::~KreogCom()
{
    if (m_next)
        delete m_next;
    std::cout << "KreogCom " << m_serial << " shutting down" << std::endl;
}

void KreogCom::addCom(int x, int y, int serial)
{
    KreogCom *newCom = new KreogCom(x, y, serial);

    if (m_next)
        newCom->m_next = m_next;
    m_next = newCom;
}

void KreogCom::removeCom()
{
    if (!m_next)
        return;

    KreogCom *toRemove = m_next;
    m_next = m_next->m_next;
    toRemove->m_next = nullptr;
    delete toRemove;
}

KreogCom *KreogCom::getCom()
{
    return m_next;
}

void KreogCom::ping() const
{
    std::cout << "KreogCom " << m_serial << " currently at " << m_x << " " << m_y << std::endl;
}

void KreogCom::locateSquad() const
{
    KreogCom* current = m_next;

    while (current) {
        current->ping();
        current = current->m_next;
    }
    ping();
}
