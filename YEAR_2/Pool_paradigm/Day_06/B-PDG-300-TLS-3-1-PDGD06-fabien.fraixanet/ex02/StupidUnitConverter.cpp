/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD06-fabien.fraixanet
** File description:
** StupidUnitConverter
*/

#include <iostream>
#include <string>
#include <iomanip>

void stupidUnitConverter(const std::string &input)
{
    double temperature;
    std::string scale;
    std::istringstream iss(input);

    if (!(iss >> temperature >> scale)) {
        return;
    }

    if (scale == "Celsius") {
        double fahrenheit = (temperature * 9.0 / 5.0) + 32;
        std::cout << std::setw(16) << std::right << std::fixed << std::setprecision(3)
        << fahrenheit << std::setw(16) << "Fahrenheit" << std::endl;
    } else if (scale == "Fahrenheit") {
        double celsius = 5.0 / 9.0 * (temperature - 32);
        std::cout << std::setw(16) << std::right << std::fixed << std::setprecision(3)
        << celsius << std::setw(16) << "Celsius" << std::endl;
    }
}

int main()
{
    std::string input;

    while (std::getline(std::cin, input)) {
        if (!input.empty()) {
            stupidUnitConverter(input);
        }
    }
    return 0;
}
