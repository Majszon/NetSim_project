// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
//
// Created by HP on 12.12.2023.
//

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "types.hpp"
#include "storage_types.hpp"
class IPackageReceiver{
    virtual void receive_package(Package && p) = 0;
    virtual ElementID get_id(void) const = 0;

};
#endif //NETSIM_NODES_HPP
// 1: Bugajski (414889), Adamek (414896), Basiura (414817)