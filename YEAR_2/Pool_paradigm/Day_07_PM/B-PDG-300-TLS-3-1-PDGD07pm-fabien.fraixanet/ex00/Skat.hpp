/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07pm-fabien.fraixanet
** File description:
** Skat
*/

#ifndef SKAT_HPP_
    #define SKAT_HPP_

    #include <iostream>
    #include <string>

class Skat {
    public:
        Skat(const std::string &name = "bob", int stimPaks = 15);
        ~Skat();

        int &stimPaks();
        const std::string &name();
        void shareStimPaks(int number, int &stock);
        void addStimPaks(unsigned int number);
        void useStimPaks();
        void status() const;

    private:
        std::string m_name;
        int m_stimPaks;
};
#endif /* !SKAT_HPP_ */
