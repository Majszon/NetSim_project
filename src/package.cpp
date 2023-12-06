//
// Created by HP on 28.11.2023.
//
#include "package.hpp"

std::set<ElementID> Package::assigned_IDs = {};
std::set<ElementID> Package::freed_IDs = {};

Package::Package() {
    if (assigned_IDs.empty() && freed_IDs.empty()) {
        ID_ = 1;
    } else if (!freed_IDs.empty()) {
        ID_ = *freed_IDs.begin();
        freed_IDs.erase(*freed_IDs.begin());
    } else if (!assigned_IDs.empty()) {
        ID_ = *assigned_IDs.end() + 1;
    }
    assigned_IDs.insert(ID_);
}

Package &Package::operator=(Package &&package)  noexcept {
    if (this == &package)
        return *this;
    assigned_IDs.erase(this->ID_);
    freed_IDs.insert(this->ID_);
    this->ID_ = package.ID_;
    assigned_IDs.insert(this->ID_);
    return *this;
}

Package::~Package() {
    freed_IDs.insert(ID_);
    assigned_IDs.erase(ID_);
}
