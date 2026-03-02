/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07am-fabien.fraixanet
** File description:
** WarpSystem
*/

#include <iostream>
#include <string>
#include "WarpSystem.hpp"

WarpSystem::QuantumReactor::QuantumReactor() : _stability(true)
{
}

WarpSystem::QuantumReactor::~QuantumReactor()
{
}

bool WarpSystem::QuantumReactor::isStable()
{
    return _stability;
}

void WarpSystem::QuantumReactor::setStability(bool stability)
{
    _stability = stability;
}

WarpSystem::Core::Core(QuantumReactor* reactor) : _coreReactor(reactor)
{
}

WarpSystem::Core::~Core()
{
}

WarpSystem::QuantumReactor* WarpSystem::Core::checkReactor()
{
    return _coreReactor;
}
