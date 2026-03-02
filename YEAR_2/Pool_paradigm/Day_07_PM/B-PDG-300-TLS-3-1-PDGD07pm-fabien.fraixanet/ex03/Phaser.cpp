/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07pm-fabien.fraixanet
** File description:
** Phaser
*/

#include "Phaser.hpp"
#include "Sounds.hpp"

Phaser::Phaser(int maxAmmo, AmmoType type) : m_maxAmmo(maxAmmo), m_defaultType(type)
{
    for (int i = 0; i < maxAmmo; ++i) {
        m_magazine.push_back(type);
    }
}

Phaser::~Phaser() {}

void Phaser::fire()
{
    if (m_magazine.empty()) {
        std::cout << "Clip empty, please reload" << std::endl;
        return;
    }

    AmmoType currentAmmo = m_magazine.front();
    m_magazine.erase(m_magazine.begin());
    
    if (currentAmmo == REGULAR) {
        std::cout << Sounds::Regular << std::endl;
    } else if (currentAmmo == PLASMA) {
        std::cout << Sounds::Plasma << std::endl;
    } else if (currentAmmo == ROCKET) {
        std::cout << Sounds::Rocket << std::endl;
    }
}

void Phaser::ejectClip()
{
    m_magazine.clear();
    std::cout << "Clip ejected" << std::endl;
}

void Phaser::changeType(AmmoType newType)
{
    m_defaultType = newType;
    std::cout << "Switching ammo to type: " << getAmmoTypeAsString(newType) << std::endl;
}

void Phaser::reload()
{
    std::cout << "Reloading..." << std::endl;
    m_magazine.clear();
    for (int i = 0; i < m_maxAmmo; ++i) {
        m_magazine.push_back(m_defaultType);
    }
}

void Phaser::addAmmo(AmmoType type)
{
    if (m_magazine.size() >= static_cast<size_t>(m_maxAmmo)) {
        std::cout << "Clip full" << std::endl;
        return;
    }
    m_magazine.push_back(type);
}

int Phaser::getCurrentAmmos() const
{
    return m_magazine.size();
}

std::string Phaser::getAmmoTypeAsString(AmmoType type) const
{
    switch (type) {
    case REGULAR: return "regular";
    case PLASMA: return "plasma";
    case ROCKET: return "rocket";
    default: return "unknown";
    }
}
