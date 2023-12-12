// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
//
// Created by HP on 12.12.2023.
//

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "types.hpp"
#include "storage_types.hpp"
class IPackageReceiver{
public:
    virtual void receive_package(Package && p) = 0;
    virtual ElementID get_id(void) const = 0;

    virtual IPackageStockPile::const_iterator begin() const = 0;
    virtual IPackageStockPile::const_iterator cbegin() const = 0;
    virtual IPackageStockPile::const_iterator end() const = 0;
    virtual IPackageStockPile::const_iterator cend() const = 0;

    virtual ~IPackageReceiver() = default;
};
#endif //NETSIM_NODES_HPP
// 1: Bugajski (414889), Adamek (414896), Basiura (414817)