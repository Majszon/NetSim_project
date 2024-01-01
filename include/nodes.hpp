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

enum class ReceiverType { // Badanie spójności, klasa Factory
    WORKER,
    STOREHOUSE
};
enum class NodeColor { UNVISITED, VISITED, VERIFIED };
class IPackageReceiver{
public:
    virtual void receive_package(Package &&  p) = 0;
    virtual ElementID get_id() const = 0;
    virtual ReceiverType get_receiver_type() const = 0;


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

    explicit ReceiverPreferences( ProbabilityGenerator pg = probability_generator) : pg_(std::move(pg)){};

    void add_receiver( IPackageReceiver* r);
    void remove_receiver( IPackageReceiver* r);

    const_iterator cbegin() const {return preferences_.cbegin(); }
    const_iterator begin() const {return preferences_.begin(); }
    const_iterator cend() const {return preferences_.cend(); }
    const_iterator end() const {return preferences_.end(); }

    IPackageReceiver *choose_receiver(void);
    const preferences_t &get_preferences() const {return preferences_;}

    ~ReceiverPreferences() = default;

private:
    preferences_t preferences_;
    ProbabilityGenerator pg_;
};
class PackageSender{
public:
    ReceiverPreferences receiver_preferences_; // obiekt preferencji odbiorcy (dla uproszczenia – jako pole publiczne)
    PackageSender() = default; //konstruktor domyślny
    PackageSender(PackageSender&&) = default; //konstruktor przenoszący (domyślny)
    void send_package(); //metoda do wysyłania produktu
    const std::optional<Package> &get_sending_buffer() const {return bufor_;}; //bufor produktu

    ~PackageSender() = default;
protected:
    void push_package(Package &&package);
private:
    std::optional<Package> bufor_ = std::nullopt;
};

class Storehouse: public IPackageReceiver{
public:
    explicit  Storehouse( ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)): id_(id), d_(std::move(d)){}

    void receive_package(Package && p) override {d_ ->push(std::move(p)); };
    ElementID get_id() const override { return id_; }

    Storehouse(Storehouse&& storehouse) = default;

    Storehouse(const Storehouse &storehouse) : id_(storehouse.get_id()) {}

    Storehouse& operator=(const Storehouse &storehouse) noexcept { id_ = storehouse.get_id(); return *this;}

    IPackageStockpile::const_iterator begin() const override { return std::begin(*d_); }
    IPackageStockpile::const_iterator cbegin() const override { return std::cbegin(*d_); }
    IPackageStockpile::const_iterator end() const override { return std::end(*d_); }
    IPackageStockpile::const_iterator cend() const override { return std::cend(*d_); }

    ReceiverType get_receiver_type() const override { return ReceiverType::STOREHOUSE; } // Badanie spójności, klasa Factory
    ~Storehouse()   = default;
private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};

class Ramp : public PackageSender{

public:
    explicit Ramp(ElementID id, TimeOffset di): PackageSender(), di_(di), id_(id) {}

    Ramp(Ramp&& ramp) = default;

    Ramp(const Ramp &ramp);

    Ramp& operator=(const Ramp &ramp) noexcept;

    TimeOffset get_delivery_interval() const { return di_; }
    ElementID get_id() const { return id_; }
    void deliver_goods(Time t);

    ~Ramp() = default;
private:
    Time t_;
    TimeOffset di_;
    std::optional<Package> bufor_ = std::nullopt;
    ElementID  id_;
};

class Worker : public IPackageReceiver, public PackageSender{
public:
    explicit Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : id_(id), pd_(pd), q_(std::move(q)) {}

    Worker() = default;

    Worker(Worker&& worker) = default;

    Worker(const Worker &worker);

    Worker& operator=(const Worker &worker) noexcept;

    void do_work(Time t);

    IPackageQueue* get_queue() const { return q_.get(); }

    TimeOffset get_processing_duration(void) const {return pd_;}
    Time get_package_processing_start_time(void) const {return t_;}
    ElementID get_id() const override {return id_;}

    ReceiverType get_receiver_type() const override { return ReceiverType::WORKER; } // Badanie spójności, klasa Factory

    void receive_package(Package &&p) override {(*q_).push(std::move(p));}

    IPackageStockpile::const_iterator cbegin() const override {return std::cbegin(*q_);}
    IPackageStockpile::const_iterator begin() const override {return std::begin(*q_);}
    IPackageStockpile::const_iterator cend() const override {return std::cend(*q_);}
    IPackageStockpile::const_iterator end() const override {return std::end(*q_);}

    const std::optional<Package> &get_processing_buffer() const {return bufor_;};

private:
    ElementID id_;
    TimeOffset pd_;
    Time t_;
    std::unique_ptr<IPackageQueue> q_;
    std::optional<Package> bufor_ = std::nullopt;
};



#endif //NETSIM_NODES_HPP
// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
