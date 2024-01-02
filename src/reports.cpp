// 1: Bugajski (414889), Adamek (414896), Basiura (414817)

#include <reports.hpp>

std::string receiver_type_to_string(ReceiverType type) {
    switch (type) {
        case ReceiverType::WORKER:
            return "worker";
        case ReceiverType::STOREHOUSE:
            return "storehouse";
        default:
            return "UNKNOWN";
    }
}

void generate_structure_report(const Factory& f, std::ostream& os) {
    os << "\n== LOADING RAMPS ==\n\n";
    for (auto it = f.ramp_cbegin(); it != f.ramp_cend(); ++it) {
        const auto &ramp = *it;
        os << "LOADING RAMP #" << ramp.get_id() << "\n";
        os << "  Delivery interval: " << ramp.get_delivery_interval() << "\n";
        os << "  Receivers:\n";

        for (const auto& receiver : ramp.receiver_preferences_.get_preferences()) {
            os << "    " << receiver_type_to_string(receiver.first->get_receiver_type()) << " #" << receiver.first->get_id() << "\n";
        }
        os << "\n";
    }

    os << "\n== WORKERS ==\n\n";
    for (auto it = f.worker_cbegin(); it != f.worker_cend(); ++it) {
        const auto& worker = *it;
        os << "WORKER #" << worker.get_id() << "\n";
        os << "  Processing time: " << worker.get_processing_duration() << "\n";
        if(worker.get_queue()->get_queue_type() == PackageQueueType::FIFO){
            os << "  Queue type: FIFO\n";
        }
        else if(worker.get_queue()->get_queue_type() == PackageQueueType::LIFO){
            os << "  Queue type: LIFO\n";
        }
        os << "  Receivers:\n";
        for (const auto& receiver : worker.receiver_preferences_.get_preferences()) {
            os << "    " << receiver_type_to_string(receiver.first->get_receiver_type()) << " #" << receiver.first->get_id() << "\n";
        }
        os << "\n";
    }

    os << "\n== STOREHOUSES ==\n\n";
    for(auto i = f.storehouse_cbegin(); i != f.storehouse_cend(); ++i) {
        const auto &storehouse = *i;
        os << "STOREHOUSE #" << storehouse.get_id() << "\n\n";
    }
}


void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t) {
    os << "=== [ Turn: " << t << " ] ===\n\n";

    os << "== WORKERS ==\n\n";
    for (auto it = f.worker_cbegin(); it != f.worker_cend(); ++it) {
        const auto& worker = *it;
        os << "WORKER #" << worker.get_id() << "\n";

        os << "  PBuffer: ";
        if (!worker.get_processing_buffer().has_value()) {
            os << "(empty)";
        } else {
            const auto& processing_buffer = worker.get_processing_buffer().value();
            os << "#" << processing_buffer.get_id() << " (pt = " << worker.get_processing_duration() << ")";
        }
        os << "\n";

        os << "  Queue: ";
        const auto& worker_queue = worker.get_queue();
        if (worker_queue->empty()) {
            os << "(empty)";
        } else {
            for (const auto& package : *worker_queue) {
                os << "#" << package.get_id();
            }
        }
        os << "\n";

        os << "  SBuffer: ";
        if (!worker.get_sending_buffer().has_value()) {
            os << "(empty)";
        } else {
            const auto& sending_buffer = worker.get_sending_buffer().value();
            os << "#" << sending_buffer.get_id();
        }
        os << "\n";
    }

    os << "\n\n== STOREHOUSES ==\n";
    for (auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); ++it) {
        const auto& storehouse = *it;
        os << "\nSTOREHOUSE #" << storehouse.get_id() << "\n";

        // Display Stock information
        os << "  Stock: ";
        if (storehouse.cbegin() == storehouse.cend()) {
            os << "(empty)";
        } else {
            for (const auto& package : storehouse) {
                os << "#" << package.get_id();
            }
        }
        os << "\n\n";
    }
}


// 1: Bugajski (414889), Adamek (414896), Basiura (414817)

