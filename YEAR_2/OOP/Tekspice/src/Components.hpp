/*
** EPITECH PROJECT, 2025
** tekspice [WSL: Ubuntu-22.04]
** File description:
** Components
*/

#pragma once

#include "AComponent.hpp"  // Fabou
#include <stdexcept>
#include <string>

// explication pour fabou

namespace nts {

// InputComponent : représente une entrée dont la valeur peut être modifiée par l'utilisateur.
class InputComponent : public AComponent {
public:
    Tristate value;

    InputComponent() : AComponent(1), value(Undefined) {}

    void simulate(std::size_t /*tick*/) override {}

    Tristate compute(std::size_t pin) override {
        if (pin != 1)
            throw std::invalid_argument("InputComponent: Pin invalide (doit être 1).");
        return value;
    }

    // Permet de modifier la valeur de l'entrée.
    void setValue(Tristate v) {
        value = v;
    }
};

// OutputComponent : représente une sortie qui affiche la valeur calculée à partir de sa liaison.
class OutputComponent : public AComponent {
public:
    OutputComponent() : AComponent(1) {}

    void simulate(std::size_t /*tick*/) override {}

    Tristate compute(std::size_t pin) override {
        if (pin != 1)
            throw std::invalid_argument("OutputComponent: Pin invalide (doit être 1).");
        return getLink(1);
    }
};

// TrueComponent : retourne toujours True.
class TrueComponent : public AComponent {
public:
    TrueComponent() : AComponent(1) {}

    void simulate(std::size_t /*tick*/) override {}

    Tristate compute(std::size_t pin) override {
        if (pin != 1)
            throw std::invalid_argument("TrueComponent: Pin invalide (doit être 1).");
        return True;
    }
};

// FalseComponent : retourne toujours False.
class FalseComponent : public AComponent {
public:
    FalseComponent() : AComponent(1) {}

    void simulate(std::size_t /*tick*/) override {}

    Tristate compute(std::size_t pin) override {
        if (pin != 1)
            throw std::invalid_argument("FalseComponent: Pin invalide (doit être 1).");
        return False;
    }
};

// ClockComponent : simule une horloge en inversant sa valeur à chaque tick de simulation.
class ClockComponent : public AComponent {
public:
    Tristate value;

    ClockComponent() : AComponent(1), value(Undefined) {}

    void simulate(std::size_t /*tick*/) override {
        // Au premier tick, la valeur Undefined devient False.
        if (value == Undefined)
            value = False;
        // Sinon, la valeur bascule.
        else if (value == False)
            value = True;
        else if (value == True)
            value = False;
    }

    Tristate compute(std::size_t pin) override {
        if (pin != 1)
            throw std::invalid_argument("ClockComponent: Pin invalide (doit être 1).");
        return value;
    }
};

}

