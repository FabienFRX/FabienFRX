/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07am-fabien.fraixanet
** File description:
** Exam
*/

#include <iostream>
#include <string>
#include "Exam.hpp"

bool Exam::cheat = false;

Exam::Exam(bool *cheatStatus)
{
    _cheatStatus = cheatStatus;
    kobayashiMaru = &Exam::start;
}

Exam::~Exam()
{
}

bool Exam::isCheating() const
{
    return *_cheatStatus;
}

void Exam::start(int vessels) {
    std::cout << "[The exam is starting]" << std::endl;
    std::cout << vessels << " Klingon vessels appeared out of nowhere." << std::endl;
    std::cout << "they are fully armed and shielded" << std::endl;
    if (cheat) {
        std::cout << "What the... someone changed the parameters of the exam !" << std::endl;
    } else {
        std::cout << "This exam is hard... you lost again." << std::endl;
    }
}
