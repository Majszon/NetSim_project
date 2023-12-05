//
// Created by HP on 28.11.2023.
//
#include <set>
#include "types.hpp"
#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP
class Package{
public:

    Package();
    Package(ElementID id): id_(id) {};
    Package(Package &&);
    ElementID get_id() const;
    Package& operator = (Package&& other); //operator przypisania przenoszącego
    ~Package();

private:
    ElementID id_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};
#endif //NETSIM_PACKAGE_HPP
