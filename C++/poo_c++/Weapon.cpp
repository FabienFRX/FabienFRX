/*
** EPITECH PROJECT, 2024
** C++
** File description:
** Weapon
*/

#include "Weapon.hpp"

using namespace std;

Weapon::Weapon() : m_name("Epee rouillee"), m_damage(10)
{

}

Weapon::Weapon(string name, int damage) : m_name(name), m_damage(damage)
{

}

void Weapon::changeWeapon(string name, int damage)
{
    m_name = name;
    m_damage = damage;
}

void Weapon::weaponDisplay() const
{
    cout << "Arme: " << m_name << " (Degats : " << m_damage << ")" << endl;
}

Weapon::~Weapon()
{
}
