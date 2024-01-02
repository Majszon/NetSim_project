// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
//
// Created by Administrator on 30.12.2023.
//
#ifndef NETSIM_REPORTS_HPP
#define NETSIM_REPORTS_HPP

#include "package.hpp"
#include "config.hpp"
#include "types.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"
#include "factory.hpp"
#include "nodes.hpp"

void generate_structure_report(const Factory& f, std::ostream& os);
void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);

class IntervalReportNotifier{
public:
    IntervalReportNotifier(TimeOffset to) : to_(to){};
    bool should_generate_report(Time t);
private:
    TimeOffset to_;
};

class SpecificTurnsReportNotifier{
public:
    SpecificTurnsReportNotifier(std::set<Time> turns) : turns_(turns){}
    bool should_generate_report(Time t);
private:
    std::set<Time> turns_;
};

#endif //NETSIM_REPORTS_HPP
// 1: Bugajski (414889), Adamek (414896), Basiura (414817)