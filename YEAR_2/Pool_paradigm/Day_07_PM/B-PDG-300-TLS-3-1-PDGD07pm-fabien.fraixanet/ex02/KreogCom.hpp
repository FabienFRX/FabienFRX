/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07pm-fabien.fraixanet
** File description:
** KreogCom
*/

#ifndef KREOGCOM_HPP_
    #define KREOGCOM_HPP_

    #include <iostream>
    #include <string>

class KreogCom {
    public:
        KreogCom(int x, int y, int serial);
        ~KreogCom();

        void addCom(int x, int y, int serial);
        void removeCom();
        KreogCom *getCom();
        void ping() const;
        void locateSquad() const;

    private:
        const int m_serial;
        int m_x;
        int m_y;
        KreogCom *m_next;
};
#endif /* !KREOGCOM_HPP_ */
