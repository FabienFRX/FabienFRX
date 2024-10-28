/*
** EPITECH PROJECT, 2024
** C++
** File description:
** Time
*/

#ifndef TIME_HPP_
#define TIME_HPP_
#include <ostream>

class Duree
{
    public:
 
    Duree(int heures = 0, int minutes = 0, int secondes = 0);
    void affiche(ostream& out) const;  //Permet d'écrire la durée dans un flux 

    private:
 
    int m_heures;
    int m_minutes;
    int m_secondes;

    friend ostream &operator<<( ostream &out, Duree const& duree );
};

#endif /* !TIME_HPP_ */
