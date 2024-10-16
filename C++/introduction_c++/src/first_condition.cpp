/*
** EPITECH PROJECT, 2024
** C++
** File description:
** first_condition
*/

#include "iostream"

using namespace std;

int first_condition()
{
    int nbAnimaux(0);

    if (nbAnimaux > 0) {
        cout << "Super ! Les animaux de compagnie ca change la vie !" << endl;
    } else {
        cout << "Vous n'avez pas d'animaux de compagnie" << endl;
    }
    cout << "Fin du programme" << endl;
    return 0;
}