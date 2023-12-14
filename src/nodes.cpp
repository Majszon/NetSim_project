//
// Created by HP on 12.12.2023.
//
#include <nodes.hpp>

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
