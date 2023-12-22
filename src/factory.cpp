// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
#include <factory.hpp>


template<typename Node>
void Factory::remove_receiver(NodeCollection<Node> &collection, ElementID id) {
    auto it = collection.find_by_id(id);
    if (it != collection.end()) {
        collection.remove_by_id(id);
    }
}

void Factory::do_work(Time t) {
    // Wykonywanie pracy przez robotnikow
    for (auto& worker : worker_) {
        worker.do_work(t);
    }
}

void Factory::do_deliveries(Time t) {
    // Dostarczanie towarów na rampy
    for (auto& ramp : ramp_) {
        ramp.deliver_goods(t);
    }
}

void Factory::do_package_passing() {
    // Przekazywanie półproduktów od nadawcy do odbiorcy
    for (auto& worker : worker_) {
        if (worker.get_sending_buffer().has_value()) {
            auto receiver = worker.receiver_preferences_.choose_receiver();
            if (receiver != nullptr) {
                receiver->receive_package((Package &&) std::move(worker.get_sending_buffer()));
            }
        }
    }
}

// BADANIE SPOJNOŚCI

bool Factory::has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {
    if (node_colors[sender] == NodeColor::VERIFIED) {
        return true;
    }

    if (node_colors[sender] == NodeColor::VISITED) {
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
    }

    node_colors[sender] = NodeColor::VERIFIED;

    if (!hasStorehouse) {
        throw std::logic_error("Sender does not have a reachable storehouse.");
    }

    return true;
}

bool Factory::is_consistent() {
    std::map<const PackageSender*, NodeColor> nodeColors;

    for (const auto& ramp : ramp_) {
        if (nodeColors[&ramp] == NodeColor::UNVISITED) {
            has_reachable_storehouse(&ramp, nodeColors);
        }
    }

    for (const auto& worker : worker_) {
        if (nodeColors[&worker] == NodeColor::UNVISITED) {
            has_reachable_storehouse(&worker, nodeColors);
        }
    }

    return true;
}


// 1: Bugajski (414889), Adamek (414896), Basiura (414817)

