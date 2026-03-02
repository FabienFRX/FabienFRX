/*
** EPITECH PROJECT, 2025
** Day_09
** File description:
** Priest
*/

#ifndef PRIEST_HPP_
    #define PRIEST_HPP_

    #include "Enchanter.hpp"

    class Priest :virtual public Enchanter {
    public:
        Priest(const std::string &name, int power);
        ~Priest();

        void rest() override;

        protected:
        private:

    };

#endif /* !PRIEST_HPP_ */
