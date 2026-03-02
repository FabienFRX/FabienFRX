/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07am-fabien.fraixanet
** File description:
** Federation
*/

#include <iostream>
#include <string>
#include "Federation.hpp"

Federation::Starfleet::Captain::Captain(const std::string &name) : _name(name), _age(0)
{
}

Federation::Starfleet::Captain::~Captain()
{
}

std::string Federation::Starfleet::Captain::getName()
{
    return _name;
}

int Federation::Starfleet::Captain::getAge()
{
    return _age;
}

void Federation::Starfleet::Captain::setAge(int age)
{
    _age = age;
}

Federation::Starfleet::Ensign::Ensign(const std::string &name) : _name(name)
{
    std::cout << "Ensign " << _name << ", awaiting orders." << std::endl;
}

Federation::Starfleet::Ensign::~Ensign()
{
}

Federation::Starfleet::Ship::Ship(int length, int width, const std::string& name, short maxWarp, int torpedo)
    : _length(length), _width(width), _name(name), _maxWarp(maxWarp), _core(nullptr), _location(EARTH), _home(EARTH), _photonTorpedo(torpedo)
{
    std::cout << "The ship USS " << _name << " has been finished." << std::endl;
    std::cout << "It is " << _length << " m in length and " << _width << " m in width." << std::endl;
    std::cout << "It can go to Warp " << _maxWarp << "!" << std::endl;
    if (_photonTorpedo != 0)
        std::cout << "Weapons are set: " << _photonTorpedo << " torpedoes ready." << std::endl;
}

Federation::Starfleet::Ship::Ship()
    : _length {289}, _width {132}, _name {(std::string)"Entreprise"}, _maxWarp {6}, _photonTorpedo {0}
{
    std::cout << "The ship USS " << _name << " has been finished." << std::endl;
    std::cout << "It is " << _length << " m in length and " << _width << " m in width." << std::endl;
    std::cout << "It can go to Warp " << _maxWarp << "!" << std::endl;
}

Federation::Starfleet::Ship::~Ship()
{
}

void Federation::Starfleet::Ship::setupCore(WarpSystem::Core* core)
{
    _core = core;
    std::cout << "USS " << _name << ": The core is set." << std::endl;
}

void Federation::Starfleet::Ship::checkCore()
{
    if (_core && _core->checkReactor()->isStable())
        std::cout << "USS " << _name << ": The core is stable at the time." << std::endl;
    else
        std::cout << "USS " << _name << ": The core is unstable at the time." << std::endl;
}

void Federation::Starfleet::Ship::promote(Captain *captain)
{
    if (captain == NULL)
        return;
    _captain = captain;
    std::cout << _captain->getName() << ": I'm glad to be the captain of the USS " << _name << "." << std::endl;
}

bool Federation::Starfleet::Ship::move(int warp, Destination d) {
    if (warp <= _maxWarp && d != _location && _core && _core->checkReactor()->isStable()) {
        _location = d;
        return true;
    }
    return false;
}

bool Federation::Starfleet::Ship::move(int warp) {
    return move(warp, _home);
}

bool Federation::Starfleet::Ship::move(Destination d) {
    return move(_maxWarp, d);
}

bool Federation::Starfleet::Ship::move() {
    return move(_maxWarp, _home);
}

int Federation::Starfleet::Ship::getShield()
{
    return _shield;
}

void Federation::Starfleet::Ship::setShield(int shield)
{
    _shield = shield;
}

int Federation::Starfleet::Ship::getTorpedo()
{
    return _photonTorpedo;
}

void Federation::Starfleet::Ship::setTorpedo(int torpedoes)
{
    _photonTorpedo = torpedoes;
}

void Federation::Starfleet::Ship::fire(Borg::Ship* target)
{
    if (target == NULL)
        return ;
    if (this->_photonTorpedo >= 1) {
        this->_photonTorpedo--;
        std::cout << this->_name << ": Firing on target. " << this->_photonTorpedo << " torpedoes remaining." << std::endl;
        target->setShield(target->getShield() - 50);
        if (this->_photonTorpedo == 0)
            std::cout << this->_name << ": No more torpedo to fire, " << this->_captain->getName() << "!" << std::endl;
    } else
        std::cout << this->_name << ": No enough torpedoes to fire, " << this->_captain->getName() << "!" << std::endl;
}

void Federation::Starfleet::Ship::fire(int torpedoes, Borg::Ship* target)
{
    if (target == NULL)
        return ;
    if (this->_photonTorpedo >= torpedoes) {
        this->_photonTorpedo -= torpedoes;
        std::cout << this->_name << ": Firing on target." << this->_photonTorpedo << " torpedoes remaining." << std::endl;
        target->setShield(target->getShield() - 50 * torpedoes);
        if (this->_photonTorpedo == 0)
            std::cout << this->_name << ": No more torpedo to fire, " << this->_captain->getName() << "!" << std::endl;
    } else
        std::cout << this->_name << ": No enough torpedoes to fire, " << this->_captain->getName() << "!" << std::endl;
}

Federation::Ship::Ship(int length, int width, const std::string& name)
    : _length(length), _width(width), _name(name), _maxWarp(1), _core(nullptr), _location(VULCAN), _home(VULCAN)
{
    std::cout << "The independent ship " << _name << " just finished its construction." << std::endl;
    std::cout << "It is " << _length << " m in length and " << _width << " m in width." << std::endl;
}

Federation::Ship::~Ship()
{
}

void Federation::Ship::setupCore(WarpSystem::Core* core)
{
    _core = core;
    std::cout << _name << ": The core is set." << std::endl;
}

void Federation::Ship::checkCore()
{
    if (_core && _core->checkReactor()->isStable())
        std::cout << _name << ": The core is stable at the time." << std::endl;
    else
        std::cout << _name << ": The core is unstable at the time." << std::endl;
}

WarpSystem::Core *Federation::Ship::getCore(void)
{
    return (_core);
}

bool Federation::Ship::move(int warp, Destination d)
{
    if (warp > _maxWarp || d == _location || !_core || !_core->checkReactor()->isStable())
        return false;
    _location = d;
    return true;
}

bool Federation::Ship::move(int warp)
{
    if (warp > _maxWarp || _home == _location || !_core || !_core->checkReactor()->isStable())
        return false;
    _location = _home;
    return true;
}

bool Federation::Ship::move(Destination d)
{
    if (d == _location || !_core || !_core->checkReactor()->isStable())
        return false;
    _location = d;
    return true;
}

bool Federation::Ship::move()
{
    if (_home == _location || !_core || !_core->checkReactor()->isStable())
        return false;
    _location = _home;
    return true;
}
