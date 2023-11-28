//
// Created by HP on 28.11.2023.
//
#include "types.hpp"
#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP
class Package{
public:
    Package(ElementID id): id(id_) {};
    Package(Package &&);
    Package & operator = (Package &&) : Package&;

    ElementID get_id() const;

    ~Package();
private:
    ElementID id_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};
#endif //NETSIM_PACKAGE_HPP
