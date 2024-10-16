/*
** EPITECH PROJECT, 2024
** C++
** File description:
** first_Math
*/

#include "iostream"
#include "cmath"

using namespace std;

int first_math()
{
    int a = 2;
    int b = 3;
    const double result = a + b;

    cout << "Donner une valeur pour a" << endl;
    cin >> a;
    cout << "Donner une valeur pour b" << endl;
    cin >> b;
    cout << a << "+" << b << endl;
    return 0;
}

int second_math()
{
   double a(0), b(0);
   double const resultat(pow(a, b));

   cout << "Bienvenue dans le programme de calcul de a^b !" << endl;
   cout << "Donnez une valeur pour a : ";
   cin >> a;
   cout << "Donnez une valeur pour b : ";
   cin >> b;
   cout << a << " ^ " << b << " = " << resultat << endl;
   return 0;
}