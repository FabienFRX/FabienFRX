/*
** EPITECH PROJECT, 2024
** C++
** File description:
** Weapon
*/

#ifndef WEAPON_HPP_
#define WEAPON_HPP_

#include <iostream>
#include <string>

class Weapon {
    public:
        Weapon();
        Weapon(std::string weaponName, int weaponDamage);
        void changeWeapon(std::string weaponName, int weaponDamage);
        void weaponDisplay()const;
        ~Weapon();

    private:
    std::string m_name;
    int m_damage;
};

#endif /* !WEAPON_HPP_ */
