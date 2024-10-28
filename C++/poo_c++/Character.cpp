/*
** EPITECH PROJECT, 2024
** C++
** File description:
** character
*/

#include "Character.hpp"
#include "Weapon.hpp"

using namespace std;

void Character::takeDamage(int nbDamage)
{
    m_life -= nbDamage;
    //On enlève le nombre de dégâts reçus à la vie du personnage

    if (m_life < 0) //Pour éviter d'avoir une vie négative
    {
        m_life = 0; //On met la vie à 0 (cela veut dire mort)
    }
}

void Character::attack(Character &target)
{
    target.takeDamage(m_weapon);
    //On inflige à la cible les dégâts que cause notre arme
}

void Character::drinkFlask(int nbFlask)
{
    m_life += nbFlask;

    if (m_life > 100) //Interdiction de dépasser 100 de vie
    {
        m_life = 100;
    }
}

bool Character::isAlive()
{
    return m_life > 0;
}

Character::Character() : m_life(100), m_mana(100), m_weapon("epee rouillee", 10)
{

}

Character::Character(string weaponName, int weaponDamage) : m_life(100), m_mana(100), m_weapon(weaponName, weaponDamage)
{

}

void Character::display_State() const
{
    cout << "Vie : " << m_life << endl;
    cout << "Mana : " << m_mana << endl;
    m_weapon.weaponDisplay();
}

Character::~Character()
{
    
}