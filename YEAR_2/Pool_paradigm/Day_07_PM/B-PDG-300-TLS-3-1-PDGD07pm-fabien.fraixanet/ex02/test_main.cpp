/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07pm-fabien.fraixanet
** File description:
** test_main
*/

#include <string>
#include <iostream>
#include "KreogCom.hpp"

int main ()
{
KreogCom k(42, 42, 101010);
k.addCom(56, 25, 65);
k.addCom(73, 34, 51);
k.locateSquad();
k.removeCom();
k.removeCom();
return 0;
}