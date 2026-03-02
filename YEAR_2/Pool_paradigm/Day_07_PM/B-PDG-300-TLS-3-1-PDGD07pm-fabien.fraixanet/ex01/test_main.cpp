/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07pm-fabien.fraixanet
** File description:
** test_main
*/

#include <iostream>
#include <string>
#include "KoalaBot.hpp"
#include "Parts.hpp"

int main ()
{
KoalaBot kb;
std :: cout << std :: boolalpha << kb. status () << std :: endl ;
kb. informations () ;
return 0;
}