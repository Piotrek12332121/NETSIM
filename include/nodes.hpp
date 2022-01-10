#include "types.hpp"

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

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
