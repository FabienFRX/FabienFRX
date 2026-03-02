/*
** EPITECH PROJECT, 2025
** Day_09
** File description:
** Paladin
*/

#ifndef PALADIN_HPP_
    #define PALADIN_HPP_

    #include "Knight.hpp"
    #include "Priest.hpp"

    class Paladin : virtual public Knight, virtual public Priest {
    public:
        Paladin(const std::string &name, int power);
        ~Paladin();

        int attack() override;
        int special() override;
        void rest() override;

        protected:
        private:

    };

#endif /* !PALADIN_HPP_ */
