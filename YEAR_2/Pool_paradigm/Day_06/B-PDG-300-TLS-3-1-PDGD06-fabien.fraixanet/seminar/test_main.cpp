/*
** EPITECH PROJECT, 2025
** Test Main for Dean, Assistant, and Student
** File description:
** main
*/

#include <iostream>
#include "Dean.hpp"
#include "Student.hpp"

int main()
{
    // Création des objets
    Student squidward("Squidward");
    Dean kaben("Kaben");

    // Début des interactions
    std::cout << "Student " << squidward.getName() << ": I'm ready to learn C++." << std::endl;
    std::cout << "Assistant 1664: 'morning everyone *sip coffee*" << std::endl;

    // Dean se présente (fait automatiquement via son constructeur)
    
    // Squidward commence à apprendre
    squidward.learn("C++");
    std::cout << "Student " << squidward.getName() << ": C++! C++! C++!" << std::endl;
    std::cout << "Assistant 1664: " << squidward.getName() << " seems fine *sip coffee*" << std::endl;

    // Squidward essaie d'apprendre plus
    std::cout << "Student " << squidward.getName() << ": Have you seen my C++ ?" << std::endl;
    std::cout << "Assistant 1664: " << squidward.getName() << " seems fine *sip coffee*" << std::endl;

    std::cout << "Student " << squidward.getName() << ": Is it shit or CPP ?" << std::endl;

    // Dean intervient pour enseigner une leçon et fournir une boisson
    kaben.teachStudent(&squidward, "C++");
    kaben.teachStudent(&squidward, "C++");
    kaben.teachStudent(&squidward, "C++");
    std::cout << "Assistant 1664: " << squidward.getName() << " needs a Red Bull *sip coffee*" << std::endl;
    std::cout << "Assistant 1664: drink this, " << squidward.getName() << " *sip coffee*" << std::endl;

    // Squidward après avoir bu
    std::cout << "Student " << squidward.getName() << ": Red Bull gives you wings!" << std::endl;
    std::cout << "Student " << squidward.getName() << ": I want to go home!" << std::endl;

    // Assistant remarque qu'il semble à nouveau normal
    std::cout << "Assistant 1664: " << squidward.getName() << " seems fine *sip coffee*" << std::endl;

    // Dean conclut la journée
    std::cout << "Assistant 1664: see you tomorrow at 9.00 *sip coffee*" << std::endl;

    // Squidward fait un dernier commentaire
    std::cout << "Student " << squidward.getName() << ": Wow, much learning today, very smart, such C++." << std::endl;

    return 0;
}
