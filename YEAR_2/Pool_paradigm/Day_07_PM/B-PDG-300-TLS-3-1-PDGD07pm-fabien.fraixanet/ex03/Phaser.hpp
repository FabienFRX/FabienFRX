/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07pm-fabien.fraixanet
** File description:
** Phaser
*/

#ifndef PHASER_HPP_
#define PHASER_HPP_

#include <iostream>
#include <string>
#include <vector>
#include "Sounds.hpp"

class Phaser {
public:
    enum AmmoType {
        REGULAR,
        PLASMA,
        ROCKET
    };

    Phaser(int maxAmmo = 20, AmmoType type = REGULAR);
    ~Phaser();

    void fire();
    void ejectClip();
    void changeType(AmmoType newType);
    void reload();
    void addAmmo(AmmoType type);
    int getCurrentAmmos() const;

private:
    static const int Empty = 0;

    int m_maxAmmo;
    AmmoType m_defaultType;
    std::vector<AmmoType> m_magazine;
    std::string getAmmoTypeAsString(AmmoType type) const;
};
#endif /* !PHASER_HPP_ */
