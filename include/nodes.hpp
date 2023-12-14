// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
//
// Created by HP on 12.12.2023.
//

//COMMITED
#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "types.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"
#include <optional>
#include <map>
enum class ReceiverType{
    Worker, Storehouse
};

class IPackageReceiver{
public:
    virtual void receive_package(Package && p) = 0;
    virtual ElementID get_id(void) const = 0;

    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;

    virtual ~IPackageReceiver() = default;
};

class ReceiverPreferences {

public:
    using preferences_t = std::map<IPackageReceiver *, double>;
    using const_iterator = preferences_t::const_iterator;

    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : pg_(std::move(pg)) {}

    const_iterator cbegin() const { return preferences_.cbegin(); }

    const_iterator cend() const { return preferences_.cend(); }

    const_iterator begin() const { return preferences_.cbegin(); }

    const_iterator end() const { return preferences_.cend(); }

    void add_receiver(IPackageReceiver *r);

    void remove_receiver(IPackageReceiver *r);

    IPackageReceiver *choose_receiver();

    const preferences_t &get_preferences() const { return preferences_; }

private:
    preferences_t preferences_;

    ProbabilityGenerator pg_;
};

class PackageSender{

public:

    ReceiverPreferences receiver_preferences_; // obiekt preferencji odbiorcy (dla uproszczenia – jako pole publiczne)

    PackageSender() = default; //konstruktor domyślny

    PackageSender(PackageSender&&) noexcept = default; //konstruktor przenoszący (domyślny)

    void send_package(); //metoda do wysyłania produktu

    const std::optional<Package> &get_sending_buffer() const {return bufor_;}; //bufor produktu

protected:

    void push_package(Package &&package);

private:
    std::optional<Package> bufor_ = std::nullopt;
};


class Ramp : public PackageSender{

public:
    Ramp(ElementID id, TimeOffset di): PackageSender(), di_(di), id_(id) {}


private:
    Time t_;
    TimeOffset di_;
    ElementID  id_;

};



