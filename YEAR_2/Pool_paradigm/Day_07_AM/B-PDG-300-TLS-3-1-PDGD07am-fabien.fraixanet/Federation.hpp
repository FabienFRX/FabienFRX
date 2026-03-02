/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07am-fabien.fraixanet
** File description:
** Federation
*/

#ifndef FEDERATION_HPP_
    #define FEDERATION_HPP_

namespace Federation {
    namespace Starfleet {
        class Captain;
        class Ensign;
        class Ship;
    };
    class Ship;
};

#include <iostream>
#include <string>
#include "WarpSystem.hpp"
#include "Destination.hpp"
#include "Borg.hpp"


namespace Federation {
    namespace Starfleet {
        class Captain {
        public:
            Captain(const std::string &name);
            ~Captain();

            std::string getName();
            int getAge();
            void setAge(int age);

        private:
            std::string _name;
            int _age;
        };

        class Ensign {
        public:
            Ensign(const std::string &name);
            ~Ensign();

        private:
            std::string _name;
        };

        class Ship {
        public:
            Ship(int length, int width, const std::string &name, short maxWarp, int torpedo = 0);
            Ship();
            ~Ship();

            void setupCore(WarpSystem::Core *core);
            void checkCore();
            void promote(Captain *captain);
            bool move(int warp, Destination d);
            bool move(int warp);
            bool move(Destination d);
            bool move();
            int getShield();
            void setShield(int shield);
            int getTorpedo();
            void setTorpedo(int torpedo);
            void fire(int torpedoes, Borg::Ship *target);
            void fire(Borg::Ship *target);

        private:
            int _length;
            int _width;
            std::string _name;
            short _maxWarp;
            WarpSystem::Core *_core;
            Captain *_captain;
            Destination _location;
            Destination _home;
            int _shield;
            int _photonTorpedo;
        };
    }
    class Ship {
    public:
        Ship(int length, int width, const std::string &name);
        ~Ship();

        void setupCore(WarpSystem::Core *core);
        void checkCore();
        WarpSystem::Core *getCore();
        bool move(int warp, Destination d);
        bool move(int warp);
        bool move(Destination d);
        bool move();

    private:
        int _length;
        int _width;
        std::string _name;
        short _maxWarp;
        WarpSystem::Core *_core;
        Destination _location;
        Destination _home;
    };
}
#endif /* !FEDERATION_HPP_ */
