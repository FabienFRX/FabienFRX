/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD10-fabien.fraixanet
** File description:
** FruitUtils
*/

#include <vector>
#include "FruitUtils.hpp"
#include "ACitrus.hpp"
#include "ABerry.hpp"

void FruitUtils::sort(FruitBox &unsorted, FruitBox &lemon, FruitBox &citrus, FruitBox &berry)
{
    FruitBox tempBox(unsorted.getSize());
    IFruit *fruit;

    while ((fruit = unsorted.popFruit()) != nullptr) {
        if (dynamic_cast<Lemon *>(fruit)) {
            if (!lemon.pushFruit(fruit)) {
                tempBox.pushFruit(fruit);
            }
        } else if (dynamic_cast<ACitrus *>(fruit)) {
            if (!citrus.pushFruit(fruit)) {
                tempBox.pushFruit(fruit);
            }
        } else if (dynamic_cast<ABerry *>(fruit)) {
            if (!berry.pushFruit(fruit)) {
                tempBox.pushFruit(fruit);
            }
        } else {
            tempBox.pushFruit(fruit);
        }
    }
    while ((fruit = tempBox.popFruit()) != nullptr) {
        unsorted.pushFruit(fruit);
    }
}

FruitBox** FruitUtils::pack(IFruit** fruits, unsigned int boxSize)
{
    if (!fruits || boxSize == 0) return nullptr;
    std::vector<FruitBox*> boxes;
    FruitBox* currentBox = nullptr;
    unsigned int i = 0;

    while (fruits[i]) {
        if (!currentBox || currentBox->getSize() == currentBox->nbFruits()) {
            currentBox = new FruitBox(boxSize);
            boxes.push_back(currentBox);
        }
        if (currentBox->pushFruit(fruits[i])) {
            i++;
        }
    }
    FruitBox** result = new FruitBox*[boxes.size() + 1];
    std::copy(boxes.begin(), boxes.end(), result);
    result[boxes.size()] = nullptr;
    return result;
}

IFruit** FruitUtils::unpack(FruitBox** fruitBoxes)
{
    if (!fruitBoxes) return nullptr;
    std::vector<IFruit*> fruits;
    IFruit* fruit = nullptr;
    unsigned int i = 0;

    while (fruitBoxes[i]) {
        while ((fruit = fruitBoxes[i]->popFruit())) {
            fruits.push_back(fruit);
        }
        i++;
    }
    IFruit** result = new IFruit*[fruits.size() + 1];
    std::copy(fruits.begin(), fruits.end(), result);
    result[fruits.size()] = nullptr;
    return result;
}
