/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD12-fabien.fraixanet
** File description:
** TestMain
*/


#include <iostream>
#include "Buzz.hpp"
#include "Woody.hpp"

int main() {
    Buzz buzz("Buzz Lightyear");
    Woody woody("Sheriff Woody");

    std::cout << buzz.getType() << " - " << buzz.getName() << std::endl;
    std::cout << woody.getType() << " - " << woody.getName() << std::endl;

    return 0;
}
