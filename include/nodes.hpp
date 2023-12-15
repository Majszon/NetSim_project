// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
//
// Created by HP on 12.12.2023.
//

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "package.hpp"
#include "config.hpp"
#include "types.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"
#include <memory>
#include <map>
#include <optional>
#include <utility>

enum class ReceiverType{
    Worker, Storehouse
};
class IPackageReceiver{
public:
    virtual void receive_package(Package && p) = 0;
    virtual ElementID get_id(void) const = 0;

    virtual IPackageStockPile::const_iterator cbegin() const = 0;
    virtual IPackageStockPile::const_iterator cend() const = 0;
    virtual IPackageStockPile::const_iterator begin() const = 0;
    virtual IPackageStockPile::const_iterator end() const = 0;

    virtual ~IPackageReceiver() = default;


};
class Worker : public IPackageReceiver, public PackageSender{
public:
    explicit Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : id_(id), pd_(pd), q_(std::move(q)) {}

    void do_work(Time t);
    TimeOffset get_processing_duration(void) const {return pd_;}
    Time get_package_processing_start_time(void) const {return t_;}
    ElementID get_id() const override {return id_;}

    void receive_package(Package &&p) override {(*q_).push(std::move(p));}

    IPackageStockPile::const_iterator cbegin() const override {return std::cbegin(*q_);}
    IPackageStockPile::const_iterator begin() const override {return std::begin(*q_);}
    IPackageStockPile::const_iterator cend() const override {return std::cend(*q_);}
    IPackageStockPile::const_iterator end() const override {return std::end(*q_);}

private:
    ElementID id_;
    TimeOffset pd_;
    Time t_;
    std::unique_ptr<IPackageQueue> q_;
    std::optional<Package> bufor_ = std::nullopt;
};
#endif //NETSIM_NODES_HPP
// 1: Bugajski (414889), Adamek (414896), Basiura (414817)