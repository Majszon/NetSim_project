// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
#include <factory.hpp>

void Factory::do_work(Time t) {
    // Wykonywanie pracy przez robotnikow
    for (auto& worker : worker_collection_) {
        worker.do_work(t);
    }
}

void Factory::do_deliveries(Time t) {
    // Dostarczanie towarów na rampy
    for (auto& ramp : ramp_collection_) {
        ramp.deliver_goods(t);
    }
}

void Factory::do_package_passing() {
    // Przekazywanie półproduktów
    for(auto& worker : worker_collection_){
        worker.send_package();
    }
    for(auto& ramp : ramp_collection_){
        ramp.send_package();
    }
}

void Factory::remove_receiver(NodeCollection<Worker>& collection, ElementID id) {
    Worker& receiver = *collection.find_by_id(id);
    for (auto &i : ramp_collection_){
        i.receiver_preferences_.remove_receiver(&receiver);
    }
}

void Factory::remove_receiver(NodeCollection<Storehouse>& collection, ElementID id) {
    Storehouse& receiver = *collection.find_by_id(id);
    for (auto &i : worker_collection_){
        i.receiver_preferences_.remove_receiver(&receiver);
    }
}

void Factory::remove_storehouse(ElementID id){
    remove_receiver(storehouse_collection_, id);
    storehouse_collection_.remove_by_id(id);
}

void Factory::remove_worker(ElementID id){
    remove_receiver(worker_collection_, id);
    worker_collection_.remove_by_id(id);
}
// BADANIE SPOJNOŚCI

bool Factory::has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {
    if (node_colors[sender] == NodeColor::VERIFIED) {
        return true;
    }

    node_colors[sender] = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().empty()) {
        throw std::logic_error("Receivers not defined.");
    }

    bool hasStorehouse = false;

    for (const auto& receiver: sender->receiver_preferences_.get_preferences()) {
        if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE) {
            hasStorehouse = true;
        } else if (receiver.first->get_receiver_type() == ReceiverType::WORKER) {
            IPackageReceiver* receiver_ptr = receiver.first;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);
            if (sendrecv_ptr == sender) {
                continue;
            }
            hasStorehouse = true;
            if (node_colors[sendrecv_ptr] == NodeColor::UNVISITED) {
                has_reachable_storehouse(sendrecv_ptr, node_colors);
            }
        }
    }

    node_colors[sender] = NodeColor::VERIFIED;

    if (!hasStorehouse) {
        throw std::logic_error("Sender does not have a reachable storehouse.");
    } else {
        return true;
    }
}

bool Factory::is_consistent() {
    std::map<const PackageSender*, NodeColor> nodeColors;

    for (const auto& ramp : ramp_collection_) {
        nodeColors[&ramp] = NodeColor::UNVISITED;
    }

    for (const auto& worker : worker_collection_) {
        nodeColors[&worker] = NodeColor::UNVISITED;
    }

    try{
        for (auto i : ramp_collection_){
            has_reachable_storehouse(&i, nodeColors);
        }
        return true;
    }
    catch(std::logic_error &e){
        return false;
    }
}

// 1: Bugajski (414889), Adamek (414896), Basiura (414817)

