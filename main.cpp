#include <iostream>
#include "nodes.hpp"
#include "storage_types.hpp"
#include "package.hpp"
#include "factory.hpp"

int main() {
    Factory factory;
    factory.add_ramp(Ramp(1, 1));
    factory.add_worker(Worker(1, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));

    Ramp& r = *(factory.find_ramp_by_id(1));
    Worker& w = *(factory.find_worker_by_id(1));
    r.receiver_preferences_.add_receiver(&w);

    Worker w2(2, 1, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    factory.find_worker_by_id(110);
}
