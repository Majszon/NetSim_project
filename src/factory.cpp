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

ParsedLineData parse_line(std::string line){
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream string_stream(line);
    std::vector<std::string> parametersKV;
    ParsedLineData Parsed;

    while (std::getline(string_stream, token, ' ')){
        tokens.push_back(token);
    }
    for (auto i : tokens){
        std::istringstream parameters(i);
        while (std::getline(parameters, token, '=')){
            parametersKV.push_back(token);
        }
    }
    std::map<std::string, std::string> map;
    for(std::size_t i = 1; i < parametersKV.size(); i += 2){
        map.emplace(parametersKV[i], parametersKV[i+1]);
    }
    if (parametersKV[0] == "RAMP"){
        Parsed.TYPE = ElementType::RAMP;

        std::istringstream id_stream(parametersKV[1]);
        std::string id;
        while(std::getline(id_stream, id, '=')){}
        std::istringstream interval_stream(parametersKV[2]);
        std::string interval;
        while(std::getline(interval_stream,interval, '=')){}
        Parsed.parse_parameters["id"] = id;
        Parsed.parse_parameters["delivery-interval"] = interval;


    }
    else if (parametersKV[0] == "WORKER"){
        Parsed.TYPE = ElementType::WORKER;

        std::istringstream id_stream(parametersKV[1]);
        std::string id;
        while(std::getline(id_stream, id, '=')){}
        std::istringstream time_stream(parametersKV[2]);
        std::string time;
        while(std::getline(time_stream, time, '=')){}
        std::istringstream queue_stream(parametersKV[3]);
        std::string queue;
        while(std::getline(queue_stream, queue, '=')){}

        Parsed.parse_parameters["id"] = id;
        Parsed.parse_parameters["processing-time"] = time;
        Parsed.parse_parameters["queue-type"] = queue;
    }
    else if (parametersKV[0] == "STOREHOUSE"){
        Parsed.TYPE = ElementType::STOREHOUSE;

        std::istringstream id_stream(parametersKV[1]);
        std::string id;
        while(std::getline(id_stream, id, '=')){}

        Parsed.parse_parameters["id"] = id;
    }
    else if (parametersKV[0] == "LINK"){
        Parsed.TYPE = ElementType::LINK;

        std::istringstream src_stream(parametersKV[1]);
        std::string src;
        while(std::getline(src_stream, src, '=')){}
        std::istringstream dest_stream(parametersKV[2]);
        std::string dest;
        while(std::getline(dest_stream, dest, '=')){}

        Parsed.parse_parameters["src"] = src;
        Parsed.parse_parameters["dest"] = dest;
    }
    else {
        Parsed.TYPE = ElementType::NONE;
    }

    return Parsed;
}

Factory load_factory_structure(std::istream& is){
    Factory factory;

    std::string line;

    while (std::getline(is, line)) {
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }
        ParsedLineData parsed_data = parse_line(line);

        switch(parsed_data.TYPE){
            case ElementType::RAMP: {
                Ramp ramp(std::stoi(parsed_data.parse_parameters["id"]), std::stoi(parsed_data.parse_parameters["delivery-interval"]));
                factory.add_ramp(std::move(ramp));
                break;
            }
            case ElementType::WORKER: {
                if(parsed_data.parse_parameters["queue-type"] == "FIFO"){
                    Worker worker(std::stoi(parsed_data.parse_parameters["id"]),std::stoi(parsed_data.parse_parameters["processing-time"]),
                                  std::make_unique<PackageQueue>(PackageQueueType::FIFO));
                    factory.add_worker(std::move(worker));
                }
                if(parsed_data.parse_parameters["queue-type"] == "LIFO"){
                    Worker worker(std::stoi(parsed_data.parse_parameters["id"]),std::stoi(parsed_data.parse_parameters["processing-time"]),
                                  std::make_unique<PackageQueue>(PackageQueueType::LIFO));
                    factory.add_worker(std::move(worker));
                }
                break;
            }
            case ElementType::STOREHOUSE:{
                Storehouse storehouse(std::stoi(parsed_data.parse_parameters["id"]));
                factory.add_storehouse(std::move(storehouse));
                break;
            }
            case ElementType::LINK:{
                // src
                std::istringstream src_stream(parsed_data.parse_parameters["src"]);
                std::string src;
                std::vector<std::string> src_v;
                while(std::getline(src_stream,src,'-')){src_v.push_back(src);}
                // dest
                std::istringstream dest_stream(parsed_data.parse_parameters["dest"]);
                std::string dest;
                std::vector<std::string> dest_v;
                while(std::getline(dest_stream,dest,'-')){dest_v.push_back(dest);}

                if(src_v[0] == "ramp"){
                    Ramp& r = *(factory.find_ramp_by_id(std::stoi(src_v[1])));
                    if(dest_v[0] == "worker"){
                        r.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(std::stoi(dest_v[1]))));

                    }
                    else if(dest_v[0] == "store"){
                        r.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(std::stoi(dest_v[1]))));

                    }
                }

                if(src_v[0] == "worker"){
                    Worker& w = *(factory.find_worker_by_id(std::stoi(src_v[1])));
                    if(dest_v[0] == "worker"){
                        w.receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(std::stoi(dest_v[1]))));
                    }
                    else if(dest_v[0] == "store"){
                        w.receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(std::stoi(dest_v[1]))));
                    }
                }
            }
            case ElementType::NONE:
                break;
        }

    }

    return factory;
}

void save_factory_structure(Factory& factory, std::ostream& os){
    os << "; == LOADING RAMPS ==\n \n";
    for(auto i = factory.ramp_cbegin(); i != factory.ramp_cend(); ++i) {
        const auto &ramp = *i;
        os << "LOADING_RAMP id=" << ramp.get_id() << " delivery-interval=" << ramp.get_delivery_interval() << "\n";
    }

    os << "\n; == WORKERS == \n\n";
    for(auto i = factory.worker_cbegin(); i != factory.worker_cend(); ++i){
        const auto& worker = *i;
        os << "WORKER id=" << worker.get_id() << " processing-time=" << worker.get_processing_duration();
        if(worker.get_queue()->get_queue_type() == PackageQueueType::FIFO){
            os << " queue-type=FIFO\n";
        }
        else if(worker.get_queue()->get_queue_type() == PackageQueueType::LIFO){
            os << " queue-type=LIFO\n";
        }
    }

    os << "\n; == STOREHOUSES == \n\n";
    for(auto i = factory.storehouse_cbegin(); i != factory.storehouse_cend(); ++i) {
        const auto &storehouse = *i;
        os << "STOREHOUSE id=" << storehouse.get_id() << "\n";
    }

    os << "\n; == LINKS ==\n\n";
    for(auto i = factory.ramp_cbegin(); i != factory.ramp_cend(); ++i) {
        const auto &ramp = *i;
        for(auto rec : ramp.receiver_preferences_.get_preferences()){
            os << "LINK src=ramp-" << ramp.get_id() << " dest=";
            if (rec.first->get_receiver_type() == ReceiverType::WORKER){
                os << "worker-" << rec.first->get_id() << "\n";
            }
            else if(rec.first->get_receiver_type() == ReceiverType::STOREHOUSE){
                os << "storehouse-" << rec.first->get_id() << "\n";
            }
        }
        os << "\n";
    }
    for(auto i = factory.worker_cbegin(); i != factory.worker_cend(); ++i) {
        const auto &worker = *i;
        for(auto rec : worker.receiver_preferences_.get_preferences()){
            os << "LINK src=worker-" << worker.get_id() << " dest=";
            if (rec.first->get_receiver_type() == ReceiverType::WORKER){
                os << "worker-" << rec.first->get_id() << "\n";
            }
            else if(rec.first->get_receiver_type() == ReceiverType::STOREHOUSE){
                os << "store-" << rec.first->get_id() << "\n";
            }
        }
        if(i==factory.worker_cend()-1){
            continue;
        }
        os << "\n";
    }

}

// 1: Bugajski (414889), Adamek (414896), Basiura (414817)

