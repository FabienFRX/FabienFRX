/*
** EPITECH PROJECT, 2025
** Paradigms Seminar
** File description:
** Day 10
*/

#pragma once

#include "IFruit.hpp"
#include "FruitBox.hpp"

class IFruitMixer
{
public:
    class IBlade
    {
    public:
        virtual ~IBlade() = default;

        virtual unsigned int operator()(const IFruit& fruit) const = 0;
    };

    virtual ~IFruitMixer() = default;

    virtual unsigned int mixFruits(FruitBox& box) const = 0;
    virtual void setBlade(IBlade* blade) = 0;
};
