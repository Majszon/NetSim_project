// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
//
// Created by HP on 05.12.2023.
//
#include "storage_types.hpp"

Package PackageQueue::pop(){
    Package pack;
    if(queue_type_ == PackageQueueType::LIFO){
        pack = std::move(queue_.back());
        queue_.pop_back();
    }
    if(queue_type_ == PackageQueueType::FIFO){
        pack = std::move(queue_.front());
        queue_.pop_front();
    }
    return pack;
}
// 1: Bugajski (414889), Adamek (414896), Basiura (414817)