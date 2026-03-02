/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD10-fabien.fraixanet
** File description:
** IFruit
*/

#ifndef IFRUIT_HPP_
    #define IFRUIT_HPP_

    #include <string>

class IFruit {
public:
    virtual ~IFruit() = default;

    virtual unsigned int getVitamins() const = 0;
    virtual std::string getName() const = 0;
    virtual bool isPeeled() const = 0;
    virtual void peel() = 0;

protected:
private:
};

std::ostream &operator<<(std::ostream &os, const IFruit &fruit);

#endif /* !IFRUIT_HPP_ */
