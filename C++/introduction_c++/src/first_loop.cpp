/*
** EPITECH PROJECT, 2024
** C++
** File description:
** fisrt_loop
*/

#include "iostream"

using namespace std;

int first_loop(int argc, const char *argv[])
{
    for (int count(0); count <= 10; count = count + 2) {
        cout << count << endl;
    }
    return 0;
}