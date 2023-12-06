//
// Created by HP on 05.12.2023.
//
#include "package.hpp"
#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP
class IPackageStockPile{
public:
    virtual void push(Package&&) = 0;
    virtual bool empty () const = 0;
    virtual size_t size() const = 0;

    
};
#endif //NETSIM_STORAGE_TYPES_HPP
