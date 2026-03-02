/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07am-fabien.fraixanet
** File description:
** Exam
*/

#ifndef EXAM_HPP_
#define EXAM_HPP_

class Exam {
    public:
        Exam(bool *cheatStatus);
        ~Exam();

        bool isCheating() const;
        void (Exam::*kobayashiMaru)(int);
        static void start(int vessels);
        static bool cheat;

    private:
        bool *_cheatStatus;
};
#endif /* !EXAM_HPP_ */
