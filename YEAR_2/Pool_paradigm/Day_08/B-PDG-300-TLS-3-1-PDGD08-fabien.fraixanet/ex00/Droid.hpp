/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD08-fabien.fraixanet
** File description:
** Droid
*/


#ifndef DROID_HPP
    #define DROID_HPP

    #include <iostream>
    #include <string>

class Droid {
    public:
        Droid(const std::string& serial = "");
        Droid(const Droid& other);
        ~Droid();

        const std::string& getId() const;
        size_t getEnergy() const;
        size_t getAttack() const;
        size_t getToughness() const;
        const std::string& getStatus() const;

        void setId(const std::string& id);
        void setEnergy(size_t energy);
        void setStatus(std::string* status);

        Droid& operator=(const Droid& other);
        bool operator==(const Droid& other) const;
        bool operator!=(const Droid& other) const;
        Droid& operator<<(size_t& energy);
        std::ostream& print(std::ostream& os) const;

    private:
        std::string m_Id;
        size_t m_Energy;
        const size_t m_Attack;
        const size_t m_Toughness;
        std::string* m_Status;
};

std::ostream& operator<<(std::ostream& os, const Droid& droid);

#endif
