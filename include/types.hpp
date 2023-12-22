// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
//
// Created by HP on 28.11.2023.
//

#ifndef NETSIM_TYPES_HPP
#define NETSIM_TYPES_HPP

#include <cstdlib>
#include <functional>
using ElementID = int;
using Time = unsigned int;
using TimeOffset = unsigned int;
using ProbabilityGenerator = std::function<double()>;
#endif //NETSIM_TYPES_HPP
// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
/*has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {
    if (node_colors[sender] == NodeColor::VERIFIED) {
        return true;
    }

    if (sender) {
        throw std::logic_error("Cycle detected in the network.");
    }

    node_colors[sender] = NodeColor::VISITED;

    bool hasStorehouse = false;

    for (const auto& receiver : sender->receiver_preferences_.get_preferences()) {
        const auto* receiverPtr = receiver.first;

        if (receiverPtr->get_receiver_type() == ReceiverType::STOREHOUSE) {
            hasStorehouse = true;
        } else {
            auto* packageSenderPtr = dynamic_cast<const PackageSender*>(receiverPtr);
            if (packageSenderPtr != nullptr) {
                hasStorehouse = hasStorehouse || has_reachable_storehouse(packageSenderPtr, node_colors);
            }
        }
    }*/