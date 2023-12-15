// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
//
// Created by HP on 12.12.2023.
//
#include "nodes.hpp"
#include "storage_types.cpp"


void ReceiverPreferences::add_receiver(IPackageReceiver *r) {
    double num_of_receivers_begin = double(preferences_.size());
    if (num_of_receivers_begin == 0) {
        preferences_[r] = 1.0;
    } else {
        for (auto &rec: preferences_) {
            rec.second = 1 / (num_of_receivers_begin + 1);
        }
        preferences_[r] = 1 / (num_of_receivers_begin + 1);
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver *r) {
    double num_of_receivers_begin = double(preferences_.size());
    if (num_of_receivers_begin > 1) {
        for (auto &rec: preferences_) {
            if (rec.first != r) {
                rec.second = 1 / (num_of_receivers_begin - 1);
            }
        }
    }
    preferences_.erase(r);
}

IPackageReceiver *ReceiverPreferences::choose_receiver() {
    auto prob = pg_();
    if (prob >= 0 && prob <= 1) {
        double distribution = 0.0;
        for (auto &rec: preferences_) {
            distribution = distribution + rec.second;
            if (distribution < 0 || distribution > 1) {
                return nullptr;
            }
            if (prob <= distribution) {
                return rec.first;
            }
        }
        return nullptr;
    }
    return nullptr;
}


void PackageSender::send_package() {
    IPackageReceiver *receiver;
    if (bufor_) {
        receiver = receiver_preferences_.choose_receiver();
        receiver->receive_package(std::move(*bufor_));
        bufor_.reset();
    }
}

void PackageSender::push_package(Package &&package) {
    bufor_.emplace(package.get_id());
}

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

void Storehouse::receive_package(Package &&p) {
    d_->push(std::move(p));
}

void Ramp::deliver_goods(Time t) {
    if (!bufor_) {
        push_package(Package());
        bufor_.emplace(id_);
        t_ = t;
    } else {
        if (t - di_ == t_) {
            push_package(Package());
        }
    }
}