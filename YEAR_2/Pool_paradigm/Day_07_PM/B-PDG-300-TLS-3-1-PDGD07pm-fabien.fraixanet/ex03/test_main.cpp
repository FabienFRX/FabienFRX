/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07pm-fabien.fraixanet
** File description:
** test_main
*/

#include <iostream>
#include <string>
#include "Phaser.hpp"


int main()
{
    Phaser p(5, Phaser::ROCKET);

    p.fire();
    p.reload();

    std::cout << "Firing all ammunitions" << std::endl;
    while (p.getCurrentAmmos() > 0)
        p.fire();

    return 0;
}