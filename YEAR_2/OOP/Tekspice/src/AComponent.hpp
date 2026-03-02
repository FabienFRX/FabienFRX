/*
** EPITECH PROJECT, 2025
** Tekspice
** File description:
** AComponent
*/

#pragma once

#include "IComponent.hpp"
#include <vector>
#include <stdexcept>

namespace nts {

class AComponent : public IComponent {
protected:
    struct Link {
        IComponent *component;
        std::size_t otherPin;
    };

    std::vector<Link> links;

public:
    AComponent(std::size_t pinCount)
        : links(pinCount + 1, {nullptr, 0})
    {
    }

    virtual ~AComponent() = default;

    void simulate(std::size_t) override {}

    void setLink(std::size_t pin, IComponent &other, std::size_t otherPin) override {
        if (pin == 0 || pin >= links.size()) {
            throw std::out_of_range("AComponent::setLink: pin out of range");
        }
        links[pin] = { &other, otherPin };
    }

    nts::Tristate getLink(std::size_t pin) const {
        if (pin == 0 || pin >= links.size()) {
            throw std::out_of_range("AComponent::getLink: pin out of range");
        }
        if (!links[pin].component) {
            return nts::Undefined;
        }
        return links[pin].component->compute(links[pin].otherPin);
    }
};

}
