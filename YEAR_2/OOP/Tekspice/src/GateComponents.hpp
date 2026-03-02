#pragma once

#include "AComponent.hpp"
#include <stdexcept>

namespace nts {

/*
  4001 : 4 portes NOR
  - Chaque porte prend 2 entrées et donne 1 sortie.
  - Par exemple, la première porte utilise les pins 1 et 2, et sort sur la pin 3.
  - Les pins 7 et 14 servent à l’alim (ignorées ici).
*/
class Nor4001Component : public AComponent {
public:
    Nor4001Component() : AComponent(14) {}

    void simulate(std::size_t /*tick*/) override {}

    Tristate compute(std::size_t pin) override {
        switch (pin) {
            case 3:  return norGate(getLink(1),  getLink(2));
            case 4:  return norGate(getLink(5),  getLink(6));
            case 10: return norGate(getLink(8),  getLink(9));
            case 11: return norGate(getLink(12), getLink(13));
            case 1: case 2: case 5: case 6:
            case 8: case 9: case 12: case 13:
                return getLink(pin);
            case 7: case 14:
                return Undefined;
            default:
                throw std::invalid_argument("Nor4001Component: pin invalide");
        }
    }

private:
    Tristate norGate(Tristate a, Tristate b) const {
        // OR
        Tristate orResult;
        if (a == True || b == True)
            orResult = True;
        else if (a == False && b == False)
            orResult = False;
        else
            orResult = Undefined;
        // NOR = inversion du résultat OR
        if (orResult == True)  return False;
        if (orResult == False) return True;
        return Undefined;
    }
};

/*
  4011 : 4 portes NAND
  - Même principe : chaque porte prend 2 entrées, sort 1 résultat (NAND).
  - Pins 7 et 14 : alim ignorée.
*/
class Nand4011Component : public AComponent {
public:
    Nand4011Component() : AComponent(14) {}

    void simulate(std::size_t /*tick*/) override {}

    Tristate compute(std::size_t pin) override {
        switch (pin) {
            case 3:  return nandGate(getLink(1),  getLink(2));
            case 4:  return nandGate(getLink(5),  getLink(6));
            case 10: return nandGate(getLink(8),  getLink(9));
            case 11: return nandGate(getLink(12), getLink(13));
            case 1: case 2: case 5: case 6:
            case 8: case 9: case 12: case 13:
                return getLink(pin);
            case 7: case 14:
                return Undefined;
            default:
                throw std::invalid_argument("Nand4011Component: pin invalide");
        }
    }

private:
    Tristate nandGate(Tristate a, Tristate b) const {
        // AND
        if (a == False || b == False)
            // AND = False => NAND = True
            return True;
        if (a == True && b == True)
            // AND = True => NAND = False
            return False;
        // Sinon Undefined
        return Undefined;
    }
};

/*
  4030 : 4 portes XOR
  - Même logique : chaque porte a 2 entrées, 1 sortie.
  - On ignore pins 7 et 14 (alim).
*/
class Xor4030Component : public AComponent {
public:
    Xor4030Component() : AComponent(14) {}

    void simulate(std::size_t /*tick*/) override {}

    Tristate compute(std::size_t pin) override {
        switch (pin) {
            case 3:  return xorGate(getLink(1),  getLink(2));
            case 4:  return xorGate(getLink(5),  getLink(6));
            case 10: return xorGate(getLink(8),  getLink(9));
            case 11: return xorGate(getLink(12), getLink(13));
            case 1: case 2: case 5: case 6:
            case 8: case 9: case 12: case 13:
                return getLink(pin);
            case 7: case 14:
                return Undefined;
            default:
                throw std::invalid_argument("Xor4030Component: pin invalide");
        }
    }

private:
    Tristate xorGate(Tristate a, Tristate b) const {
        if (a == Undefined || b == Undefined)
            return Undefined;
        // XOR = True si a != b
        return (a != b) ? True : False;
    }
};

/*
  4069 : 6 inverseurs (NOT)
  - Chaque inverseur prend 1 entrée et sort 1 sortie.
  - Les pins sont organisées par paires : (1->2), (3->4), (5->6), etc.
*/
class Not4069Component : public AComponent {
public:
    Not4069Component() : AComponent(14) {}

    void simulate(std::size_t /*tick*/) override {}

    Tristate compute(std::size_t pin) override {
        switch (pin) {
            case 2:  return invert(getLink(1));
            case 4:  return invert(getLink(3));
            case 6:  return invert(getLink(5));
            case 8:  return invert(getLink(9));
            case 10: return invert(getLink(11));
            case 12: return invert(getLink(13));
            case 1: case 3: case 5: case 9: case 11: case 13:
                return getLink(pin);
            case 7: case 14:
                return Undefined;
            default:
                throw std::invalid_argument("Not4069Component: pin invalide");
        }
    }

private:
    Tristate invert(Tristate in) const {
        if (in == Undefined)
            return Undefined;
        return (in == True) ? False : True;
    }
};

/*
  4071 : 4 portes OR
  - Chaque porte prend 2 entrées, sort 1 résultat (OR).
  - Pins 7 et 14 pour l’alim, ignorés.
*/
class Or4071Component : public AComponent {
public:
    Or4071Component() : AComponent(14) {}

    void simulate(std::size_t /*tick*/) override {}

    Tristate compute(std::size_t pin) override {
        switch (pin) {
            case 3:  return orGate(getLink(1),  getLink(2));
            case 4:  return orGate(getLink(5),  getLink(6));
            case 10: return orGate(getLink(8),  getLink(9));
            case 11: return orGate(getLink(12), getLink(13));
            case 1: case 2: case 5: case 6:
            case 8: case 9: case 12: case 13:
                return getLink(pin);
            case 7: case 14:
                return Undefined;
            default:
                throw std::invalid_argument("Or4071Component: pin invalide");
        }
    }

private:
    Tristate orGate(Tristate a, Tristate b) const {
        if (a == True || b == True)
            return True;
        if (a == False && b == False)
            return False;
        return Undefined;
    }
};

/*
  4081 : 4 portes AND
  - Chaque porte prend 2 entrées, sort 1 résultat (AND).
  - Même logique, on ignore pins 7 et 14.
*/
class And4081Component : public AComponent {
public:
    And4081Component() : AComponent(14) {}

    void simulate(std::size_t /*tick*/) override {}

    Tristate compute(std::size_t pin) override {
        switch (pin) {
            case 3:  return andGate(getLink(1),  getLink(2));
            case 4:  return andGate(getLink(5),  getLink(6));
            case 10: return andGate(getLink(8),  getLink(9));
            case 11: return andGate(getLink(12), getLink(13));
            case 1: case 2: case 5: case 6:
            case 8: case 9: case 12: case 13:
                return getLink(pin);
            case 7: case 14:
                return Undefined;
            default:
                throw std::invalid_argument("And4081Component: pin invalide");
        }
    }

private:
    Tristate andGate(Tristate a, Tristate b) const {
        if (a == False || b == False)
            return False;
        if (a == True && b == True)
            return True;
        return Undefined;
    }
};

}
