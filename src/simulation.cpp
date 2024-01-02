// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
//
// Created by HP on 02.01.2024.
//

#include "simulation.hpp"

void simulate(Factory &f, TimeOffset d, std::function<void (Factory&, Time)> rf) {
    for (Time t = 1; t <= d; t++) {
        if (!f.is_consistent()) {
            throw std::logic_error("Cannot simulate: inconsistency occured.");
        }
        f.do_deliveries(t);
        f.do_package_passing();
        f.do_work(t);
        rf(f,t);
    }
}
// 1: Bugajski (414889), Adamek (414896), Basiura (414817)