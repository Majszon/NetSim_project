// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
//
// Created by HP on 28.11.2023.
//
#include "package.hpp"

std::set<ElementID> Package::assigned_IDs = {};
std::set<ElementID> Package::freed_IDs = {};

Package::Package() {
    if (assigned_IDs.empty() && freed_IDs.empty()) {
        id_ = 1;
    } else if (!freed_IDs.empty()) {
        id_ = *freed_IDs.begin();
        freed_IDs.erase(*freed_IDs.begin());
    } else if (!assigned_IDs.empty()) {
        id_ = *assigned_IDs.end() + 1;
    }
    assigned_IDs.insert(id_);
}

Package &Package::operator=(Package &&package)  noexcept {
    if (this == &package)
        return *this;
    assigned_IDs.erase(this->id_);
    freed_IDs.insert(this->id_);
    this->id_ = package.id_;
    assigned_IDs.insert(this->id_);
    return *this;
}

Package::~Package() {
    freed_IDs.insert(id_);
    assigned_IDs.erase(id_);
}
// 1: Bugajski (414889), Adamek (414896), Basiura (414817)