// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
//
// Created by HP on 19.12.2023.
//

#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP

#include "nodes.hpp"
#include <stdexcept>
template <typename Node>
class NodeCollection{
public:
    using list = typename std::list<Node>;
    using iterator = typename list::iterator;
    using const_iterator = typename list::const_iterator;

    NodeCollection() = default;
    void add(Node&& node) { collection_.emplace_back(std::move(node)); }
    void remove_by_id(ElementID id) { collection_.erase(NodeCollection<Node>::find_by_id(id)); }
    NodeCollection<Node>::iterator find_by_id(ElementID id) {return std::find_if(collection_.begin(), collection_.end(), [id](Node &elem){return elem.get_id() == id; }); }
    NodeCollection<Node>::const_iterator find_by_id(ElementID id) const {return std::find_if(collection_.cbegin(), collection_.cend(), [id](const Node &elem){return elem.get_id() == id; }); }


    const_iterator cbegin() const { return collection_.cbegin(); }
    const_iterator cend() const { return collection_.cend(); }
    const_iterator begin() const { return collection_.begin(); }
    const_iterator end() const { return collection_.end(); }

    iterator begin() { return collection_.begin(); }
    iterator end() { return collection_.end(); }

    ~NodeCollection() = default;
private:
    list collection_;
};

class Factory{
public:

    void add_ramp(Ramp &&ramp) { ramp_.add(std::move(ramp)); }
    void remove_ramp(ElementID id) { ramp_.remove_by_id(id); }
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return ramp_.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return ramp_.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const {return ramp_.cbegin(); }
    NodeCollection<Ramp>::const_iterator ramp_cend() const {return ramp_.cend(); }

    void add_worker(Worker &&worker) { worker_.add(std::move(worker)); }
    void remove_worker(ElementID id) { worker_.remove_by_id(id); }
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return worker_.find_by_id(id); }
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const {return worker_.find_by_id(id); }
    NodeCollection<Worker>::const_iterator worker_cbegin() const {return worker_.cbegin(); }
    NodeCollection<Worker>::const_iterator worker_cend() const {return worker_.cend(); }

    void add_storehouse(Storehouse &&storehouse) { storehouse_.add(std::move(storehouse)); }
    void remove_storehouse(ElementID id) { storehouse_.remove_by_id(id) ;}
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { return storehouse_.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { return storehouse_.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return storehouse_.cbegin(); }
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return storehouse_.cend(); }

    void do_work(Time t);
    void do_deliveries(Time t);
    void do_package_passing();
    bool is_consistent();


private:
    NodeCollection<Ramp> ramp_;
    NodeCollection<Worker> worker_;
    NodeCollection<Storehouse> storehouse_;
    template <typename Node>
    void remove_receiver(NodeCollection<Node>&collection, ElementID id);
    bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);
};

#endif //NETSIM_FACTORY_HPP
// 1: Bugajski (414889), Adamek (414896), Basiura (414817)

