/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07am-fabien.fraixanet
** File description:
** BorgQueen
*/

#ifndef BORGQUEEN_HPP
    #define BORGQUEEN_HPP

    #include "Borg.hpp"
    #include "Federation.hpp"

namespace Borg {
    class BorgQueen {
    public:
        BorgQueen();
        ~BorgQueen();
        bool move(Ship *ship, Destination dest);
        void fire(Ship *ship, Federation::Starfleet::Ship *target);
        void destroy(Ship *ship, Federation::Ship *target);

    private:
        bool (Borg::Ship::*movePtr)(Destination);
        void (Borg::Ship::*firePtr)(Federation::Starfleet::Ship *);
        void (Borg::Ship::*destroyPtr)(Federation::Ship *);
    };
}
#endif
