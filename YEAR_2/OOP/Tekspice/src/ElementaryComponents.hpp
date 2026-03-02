/*
** EPITECH PROJECT, 2025
** a
** File description:
** ElementaryComponents
*/

#pragma once

#include "AComponent.hpp"  // Fabou
#include <stdexcept>

// explication pour fabou

namespace nts {

// AndComponent : implémente une porte AND avec deux entrées (pins 1 et 2) et une sortie (pin 3).
class AndComponent : public AComponent {
public:
    AndComponent() : AComponent(3) {}

    void simulate(std::size_t /*tick*/) override {}

    Tristate compute(std::size_t pin) override {
        if (pin == 3) {
            Tristate in1 = getLink(1);
            Tristate in2 = getLink(2);
            // Si l'une des entrées est False, le résultat est False.
            if (in1 == False || in2 == False)
                return False;
            // Si les deux entrées sont True, le résultat est True.
            if (in1 == True && in2 == True)
                return True;
            // Sinon, le résultat reste Undefined.
            return Undefined;
        } else if (pin == 1 || pin == 2) {
            return getLink(pin);
        }
        throw std::invalid_argument("AndComponent: Pin invalide.");
    }
};

// OrComponent : implémente une porte OR avec deux entrées (pins 1 et 2) et une sortie (pin 3).
class OrComponent : public AComponent {
public:
    OrComponent() : AComponent(3) {}

    void simulate(std::size_t /*tick*/) override {}

    Tristate compute(std::size_t pin) override {
        if (pin == 3) {
            Tristate in1 = getLink(1);
            Tristate in2 = getLink(2);
            // Si l'une des entrées est True, le résultat est True.
            if (in1 == True || in2 == True)
                return True;
            // Si les deux entrées sont False, le résultat est False.
            if (in1 == False && in2 == False)
                return False;
            return Undefined;
        } else if (pin == 1 || pin == 2) {
            return getLink(pin);
        }
        throw std::invalid_argument("OrComponent: Pin invalide.");
    }
};

// XorComponent : implémente une porte XOR avec deux entrées (pins 1 et 2) et une sortie (pin 3).
class XorComponent : public AComponent {
public:
    XorComponent() : AComponent(3) {}

    void simulate(std::size_t /*tick*/) override {}

    Tristate compute(std::size_t pin) override {
        if (pin == 3) {
            Tristate in1 = getLink(1);
            Tristate in2 = getLink(2);
            // Si l'une des entrées est Undefined, le résultat est Undefined.
            if (in1 == Undefined || in2 == Undefined)
                return Undefined;
            // Le résultat est True si les entrées diffèrent, sinon False.
            return (in1 != in2) ? True : False;
        } else if (pin == 1 || pin == 2) {
            return getLink(pin);
        }
        throw std::invalid_argument("XorComponent: Pin invalide.");
    }
};

// NotComponent : implémente un inverseur avec une entrée (pin 1) et une sortie (pin 2).
class NotComponent : public AComponent {
public:
    NotComponent() : AComponent(2) {}

    void simulate(std::size_t /*tick*/) override {}

    Tristate compute(std::size_t pin) override {
        if (pin == 2) {
            Tristate in = getLink(1);
            if (in == Undefined)
                return Undefined;
            return (in == True) ? False : True;
        } else if (pin == 1) {
            return getLink(1);
        }
        throw std::invalid_argument("NotComponent: Pin invalide.");
    }
};

}
