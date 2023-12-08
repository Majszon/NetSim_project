// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
//
// Created by HP on 28.11.2023.
//
#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include "types.hpp"
#include <set>


class Package {
public:
    Package();

    explicit Package(ElementID id) : id_(id) { assigned_IDs.insert(id_); }

    Package(Package &&package)  noexcept : id_(package.id_) {}

    Package &operator=(Package &&package) noexcept ;

    ElementID get_id() const { return id_; }

    ~Package();

private:
    ElementID id_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};


#endif //NETSIM_PACKAGE_HPP
// 1: Bugajski (414889), Adamek (414896), Basiura (414817)