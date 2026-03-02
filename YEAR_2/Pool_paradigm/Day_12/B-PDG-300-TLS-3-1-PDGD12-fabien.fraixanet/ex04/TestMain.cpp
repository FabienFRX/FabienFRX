/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD12-fabien.fraixanet
** File description:
** TestMais
*/

#include <iostream>
#include <memory>
#include "Buzz.hpp"
#include "Woody.hpp"

int main()
{
    Toy a(Toy::BASIC_TOY, "REX", "rex.txt");

    std::cout << a;
    a << "rawr xD";
    std::cout << a;
}
