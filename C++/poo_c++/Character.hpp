/*
** EPITECH PROJECT, 2024
** C++
** File description:
** character
*/

#ifndef CHARACTER_HPP_
#define CHARACTER_HPP_

#include <string>
#include "Weapon.hpp"

class Character
{
    public:

    Character();
    Character(std::string weaponName, int weaponDamage);
    void takeDamage(int nbDamage);
    void attack(Character &target);
    void drinkFlask(int nbFlask);
    bool isAlive();
    void display_State() const;
    ~Character();

    private:

    int m_life;
    int m_mana;
    Weapon m_weapon;

};
#endif /* !CHARACTER_HPP_ */
