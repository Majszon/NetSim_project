// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
//
// Created by HP on 19.12.2023.
//

#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP

#include "nodes.hpp"
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <iostream>

template <typename Node> class NodeCollection{
public:
    using list = typename std::list<Node>;
    using iterator = typename list::iterator;
    using const_iterator = typename list::const_iterator;

    void add(Node&& node) { collection_.emplace_back(std::move(node)); }
    void remove_by_id(ElementID id) {
        auto iter = find_by_id(id);
        if (iter != collection_.end()){
        collection_.erase(find_by_id(id)); }}
    NodeCollection<Node>::iterator find_by_id(ElementID id) {return std::find_if(collection_.begin(), collection_.end(), [id](auto& node){return node.get_id() == id; }); }
    NodeCollection<Node>::const_iterator find_by_id(ElementID id) const {return std::find_if(collection_.cbegin(), collection_.cend(), [id](const auto& node){return node.get_id() == id; }); }


    const_iterator cbegin() const { return collection_.cbegin(); }
    const_iterator cend() const { return collection_.cend(); }
    const_iterator begin() const { return collection_.begin(); }
    const_iterator end() const { return collection_.end(); }

    iterator begin() { return collection_.begin(); }
    iterator end() { return collection_.end(); }

private:
    list collection_;
};

class Factory{
public:

    void add_ramp(Ramp &&ramp) { ramp_collection_.add(std::move(ramp)); }
    void remove_ramp(ElementID id) { ramp_collection_.remove_by_id(id); }
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return ramp_collection_.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return ramp_collection_.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const {return ramp_collection_.cbegin(); }
    NodeCollection<Ramp>::const_iterator ramp_cend() const {return ramp_collection_.cend(); }

    void add_worker(Worker &&worker) { worker_collection_.add(std::move(worker)); }
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return worker_collection_.find_by_id(id); }
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const {return worker_collection_.find_by_id(id); }
    NodeCollection<Worker>::const_iterator worker_cbegin() const {return worker_collection_.cbegin(); }
    NodeCollection<Worker>::const_iterator worker_cend() const {return worker_collection_.cend(); }

    void add_storehouse(Storehouse &&storehouse) { storehouse_collection_.add(std::move(storehouse)); }
    void remove_storehouse(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { return storehouse_collection_.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { return storehouse_collection_.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return storehouse_collection_.cbegin(); }
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return storehouse_collection_.cend(); }

    void do_work(Time t);
    void do_deliveries(Time t);
    void do_package_passing();
    bool is_consistent();
    bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);

private:
    NodeCollection<Ramp> ramp_collection_;
    NodeCollection<Worker> worker_collection_;
    NodeCollection<Storehouse> storehouse_collection_;

    void remove_receiver(NodeCollection<Worker>& collection, ElementID id);

    void remove_receiver(NodeCollection<Storehouse>& collection, ElementID id);
};
enum class ElementType{
    RAMP, WORKER, STOREHOUSE, LINK, NONE
};
class ParsedLineData{
public:

    ParsedLineData(ElementType elementType, std::map<std::string, std::string> &&parameters) : type_(elementType), parse_parameters_(parameters) {}

    ElementType get_element_type() const { return type_; }
    std::map<std::string, std::string> get_parameters() const { return parse_parameters_; }

    ElementType type_;
    std::map<std::string,std::string> parse_parameters_;
};

ParsedLineData parse_line(std::string line);
Factory load_factory_structure(std::istream& is);
void save_factory_structure(Factory& factory, std::ostream& os);

#endif //NETSIM_FACTORY_HPP
// 1: Bugajski (414889), Adamek (414896), Basiura (414817)

