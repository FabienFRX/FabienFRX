/*
** EPITECH PROJECT, 2024
** C++
** File description:
** referenceCode
*/

#include "iostream"

using namespace std;

int reference_code()
{
    int ageUtilisateur(18);
    int& maReference(ageUtilisateur); //Et une référence sur la variable 'ageUtilisateur'

    cout << "Vous avez " << ageUtilisateur << "  ans. (via variable)" << endl;
    cout << "Vous avez " << maReference << " ans. (via reference)" << endl;
    return 0;
}