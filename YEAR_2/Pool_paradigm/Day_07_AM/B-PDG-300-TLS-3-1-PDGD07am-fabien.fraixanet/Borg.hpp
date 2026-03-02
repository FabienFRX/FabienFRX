/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07am-fabien.fraixanet
** File description:
** Borg
*/

#ifndef BORG_HPP_
    #define BORG_HPP_

namespace Borg {
    class Ship;
};

    #include <iostream>
    #include <string>
    #include "WarpSystem.hpp"
    #include "Destination.hpp"
    #include "Federation.hpp"

namespace Borg {
    class Ship {
    public:
        Ship(int weaponFrequency = 20, short repair = 3);
        ~Ship();

        void setupCore(WarpSystem::Core *core);
        void checkCore();
        bool move(int warp, Destination d);
        bool move(int warp);
        bool move(Destination d);
        bool move();
        int getShield();
        void setShield(int shield);
        int getWeaponFrequency();
        void setWeaponFrequency(int frequency);
        short getRepair();
        void setRepair(short repair);
        void fire(Federation::Starfleet::Ship *target);
        void fire(Federation::Ship *target);
        void repair();

    private:
        int _side;
        short _maxWarp;
        WarpSystem::Core *_core;
        Destination _location;
        Destination _home;
        int _shield;
        int _weaponFrequency;
        short _repair;
    };
}
#endif /* !BORG_HPP_ */
