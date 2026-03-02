#ifndef ADMIRAL_HPP
    #define ADMIRAL_HPP

    #include <string>
    #include "Borg.hpp"
    #include "Federation.hpp"

namespace Federation {
    namespace Starfleet {
        class Admiral {
        public:
            Admiral(const std::string &name);
            ~Admiral();

            void (Starfleet::Ship::*firePtr)(Borg::Ship *);
            bool (Starfleet::Ship::*movePtr)(Destination);
            void fire(Starfleet::Ship *ship, Borg::Ship *target);
            bool move(Starfleet::Ship *ship, Destination dest);

        private:
            std::string _name;
        };
    }
}
#endif
