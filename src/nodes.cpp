//
// Created by HP on 12.12.2023.
//
#include "nodes.hpp"


void Worker::do_work(Time t) {
    if (!std::empty(*q_) && !bufor_ ) {
        bufor_.emplace(pop(*q_));
        t_ = t;
    }
    else {
        if (t - t_ + 1 == pd_) {
            push_package(Package(bufor_.value().get_id()));
            bufor_.reset();
            if (!std::empty(*q_)) {
                bufor_.emplace(pop(*q_));
            }
        }
    }
}