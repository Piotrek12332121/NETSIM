#include "types.hpp"

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

enum class ReceiverType{
    WORKER = 0,
    STOREHOUSE = 1
};


class IPackageReceiver{
public:
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;

    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;
    virtual ~IPackageReceiver() {};


    virtual ReceiverType get_receiver_type() const = 0;
};





class Storehouse : public IPackageReceiver{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d =
            std::make_unique<PackageQueue>(PackageQueueType::LIFO)) :
            id_(id), ptr_(std::move(d)) {};

    Storehouse(Storehouse&&) = default;

    IPackageStockpile::const_iterator begin() const override {return ptr_->begin();}
    IPackageStockpile::const_iterator cbegin() const override {return ptr_->cbegin();}
    IPackageStockpile::const_iterator end() const override {return ptr_->end();}
    IPackageStockpile::const_iterator cend() const override {return ptr_->cend();}

    void receive_package(Package&& p) override {ptr_->push(std::move(p));}
    ElementID get_id() const override {return id_;}
    ~Storehouse() override = default;


    ReceiverType get_receiver_type() const override {return ReceiverType::STOREHOUSE;}
private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> ptr_;
};




class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {}
    TimeOffset get_delivery_interval() const {return di_;} // Getter offsetu
    ElementID get_id() const {return id_;} // Getter ID
    void deliver_goods(Time t) const { if(t % di_ == 0) { send_package(); } }
private:
    ElementID id_; // ID rampy
    TimeOffset di_; // Czas wykonania pojedyńczej wysyłki
};

#endif //NETSIM_NODES_HPP
