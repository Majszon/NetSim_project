// 1: Bugajski (414889), Adamek (414896), Basiura (414817)
//
// Created by HP on 12.12.2023.
//

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "types.hpp"
#include "storage_types.hpp"
#include <optional>

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

class PackageSender{
//Klasa PackageSender powinna przechować obiekt preferencji odbiorcy (dla uproszczenia – jako pole publiczne)
// oraz posiadać metodę do “wysyłania” półproduktu.
//
//Warto dodać też pole realizujące funkcjonalność bufora półproduktu gotowego do wysłania
// – dzięki temu istnieje możliwość opracowania jednej wspólnej implementacji metody wysyłającej półprodukt
// dla wszelkich możliwych typów nadawców półproduktów (nadawca umieszcza półprodukt w buforze za pomocą metody udostępnianej
// przez klasę PackageSender, a metoda wysyłająca przekazuje zawartość bufora dalej – do odbiorcy).
//
//Konstruktor przenoszący PackageSender(PackageSender&&) powinien mieć domyślną implementację.

public:

    ReceiverPreferences = receiver_preferences_

    PackageSender() = default;

    PackageSender(PackageSender&&); //konstruktor przenoszący

    void send_package(void);

    const std::optional<Package> &get_sending_buffer() const {return bufor_;};

private:
    std::optional<Package> bufor_ = std::nullopt;
};


class Ramp{
public:

};


class ReceiverPreferences{

private:
    preferences_t preferences;
};

//test komentarz
#endif //NETSIM_NODES_HPP
// 1: Bugajski (414889), Adamek (414896), Basiura (414817)