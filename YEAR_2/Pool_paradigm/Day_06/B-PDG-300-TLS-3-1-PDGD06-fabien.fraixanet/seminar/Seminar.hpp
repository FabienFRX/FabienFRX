/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD06-fabien.fraixanet
** File description:
** Seminar
*/

#ifndef SEMINAR_HPP_
    #define SEMINAR_HPP_

    #include "Student.hpp"
    #include "Assistant.hpp"
    #include "Dean.hpp"

class Seminar {
    public:
        Seminar();
        ~Seminar();

        void add(Dean* dean);
        void addAssistant(Assistant* assistant);
        void addStudent(Student* student);
        void run();

    private:
        
};
#endif /* !SEMINAR_HPP_ */
