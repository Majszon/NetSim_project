// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
//
// Created by HP on 02.01.2024.
//

#ifndef NETSIM_SIMULATION_HPP
#define NETSIM_SIMULATION_HPP

#include "factory.hpp"

void simulate(Factory &f, TimeOffset d, std::function<void (Factory&, Time)> rf);

#endif //NETSIM_SIMULATION_HPP
// 1: Bugajski (414889), Adamek (414896), Basiura (414817)