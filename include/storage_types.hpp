//
// Created by Mateusz Basiura on 05/12/2023.
//
#include "package.hpp"
#include <list>
#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

class PackageQueue : public IPackageQueue{
public:


private:
    std::list<Package>


};

enum class PackageQueueType{
    FIFO,
    LIFO
};


#endif //NETSIM_STORAGE_TYPES_HPP
