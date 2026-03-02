/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD10-fabien.fraixanet
** File description:
** Test_main
*/

#include <iostream>
#include <string>
#include "ABerry.hpp"
#include "ACitrus.hpp"
#include "AFruit.hpp"
#include "Almond.hpp"
#include "ANut.hpp"
#include "IFruit.hpp"
#include "Lemon.hpp"
#include "Orange.hpp"
#include "Strawberry.hpp"
#include "FruitBox.hpp"
#include "FruitUtils.hpp"

int main()
{
    // Création des fruits
    IFruit* fruits[] = {
        new Lemon(), new Orange(), new Strawberry(),
        new Lemon(), new Orange(), nullptr
    };

    // Packing des fruits
    FruitBox** boxes = FruitUtils::pack(fruits, 2);
    std::cout << "Packing complete!" << std::endl;

    // 🔽 **Ajoute ce bloc ici pour vérifier le contenu des boîtes après packing**
    std::cout << "Checking packed boxes:" << std::endl;
    int boxIndex = 0;
    while (boxes[boxIndex]) {
        std::cout << "Box " << boxIndex + 1 << " contains " << boxes[boxIndex]->nbFruits() << " fruits" << std::endl;
        boxIndex++;
    }

    // Unpacking des fruits
    IFruit** unpackedFruits = FruitUtils::unpack(boxes);
    std::cout << "Unpacking complete!" << std::endl;

    // 🔽 **Ajoute ce bloc ici pour vérifier le contenu après unpacking**
    std::cout << "Checking unpacked fruits:" << std::endl;
    int fruitIndex = 0;
    while (unpackedFruits[fruitIndex]) {
        std::cout << "Fruit " << fruitIndex + 1 << " is still present" << std::endl;
        fruitIndex++;
    }

    // Libération mémoire
    for (int i = 0; boxes[i] != nullptr; i++) {
        delete boxes[i];
    }
    delete[] boxes;
    delete[] unpackedFruits;

    return 0;
}
