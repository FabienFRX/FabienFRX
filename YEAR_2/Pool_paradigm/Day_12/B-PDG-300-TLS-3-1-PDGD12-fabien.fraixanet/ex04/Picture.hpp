/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD12-fabien.fraixanet
** File description:
** Picture
*/

#ifndef PICTURE_HPP_
    #define PICTURE_HPP_

    #include <string>

class Picture {
public:
    std::string data;

    Picture(const std::string &file = "");
    bool getPictureFromFile(const std::string &file);
};
#endif /* !PICTURE_HPP_ */
