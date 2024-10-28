/*
** EPITECH PROJECT, 2024
** C++
** File description:
** main
*/

#include <iostream>
#include "Character.hpp"
#include "Weapon.hpp"
#include "Time.hpp"

using namespace std;

int main()
{
    Character goliath("Epee aiguisee", 20);
    Character david;

    Duree time1(0, 10, 28), time2(0, 15, 2);
    goliath.attack(david);
    david.drinkFlask(20);
    goliath.attack(david);
    david.attack(goliath);
    goliath.changeWeapon("Double hache tranchante veneneuse de la mort", 40);
    goliath.attack(david);

    cout << "David" << endl;
    david.display_State();
    cout << endl << "Goliath" << endl;
    goliath.display_State();
    return 0;
}