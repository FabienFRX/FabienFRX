/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07pm-fabien.fraixanet
** File description:
** test_main
*/

#include <iostream>
#include <string>
#include "Skat.hpp"

int main ()
{
Skat s("Junior", 5);
std::cout << "Soldier " << s.name() << std::endl;
s.status();
s.useStimPaks();
return 0;
}
