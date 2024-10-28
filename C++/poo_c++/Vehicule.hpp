/*
** EPITECH PROJECT, 2024
** C++
** File description:
** Vehicule
*/

#ifndef VEHICULE_HPP_
#define VEHICULE_HPP_

class Vehicule
{
    public:
    Vehicule(int prix);
    virtual void affiche() const;
    virtual int nbrRoues() const = 0; //Affiche le nombre de roues du véhicule
    virtual ~Vehicule();

    protected:
    int m_prix;
};

class Voiture : public Vehicule
{
    public:
    Voiture(int prix, int portes);
    virtual void affiche() const;
    virtual int nbrRoues() const; //Affiche le nombre de roues de la voiture
    virtual ~Voiture();

    private:
    int m_portes;
};
#endif /* !VEHICULE_HPP_ */
