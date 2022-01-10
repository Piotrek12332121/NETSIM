#include "factory.hpp"
#include "nodes.hpp"

#include <stdexcept>

void Factory::remove_worker(ElementID id) {
    for (auto& ramp : ramps_) {
        for (auto p : ramp.receiver_preferences_.get_preferences()) {
            if (p.first->get_id() == id && p.first->get_receiver_type() == ReceiverType::WORKER) {
                ramp.receiver_preferences_.remove_receiver(p.first);
                break;
            }
        }
    }
    workers_.remove_by_id(id);
}

enum class NodeColor { UNVISITED, VISITED, VERIFIED };

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {
    if (node_colors.at(sender) == NodeColor::VERIFIED) {
        return true;
    }
    node_colors.at(sender) = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().empty()) {
        throw std::logic_error("Receivers not defined");
    }

    bool different_receiver_than_itself = false;
    for (const auto& receiver : sender->receiver_preferences_) {
        if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE) {
            different_receiver_than_itself = true;
        }
        else {
            IPackageReceiver* receiver_ptr = receiver.first;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if (sendrecv_ptr == sender) {
                continue;
            }
            different_receiver_than_itself = true;

            if (node_colors.at(sendrecv_ptr) != NodeColor::VISITED) {
                has_reachable_storehouse(sendrecv_ptr, node_colors);
            }
        }
    }


    bool Factory::is_consistent() {

        std::map<const PackageSender*, NodeColor> color;

        for (auto &r : ramps_list_) {
            color.insert(std::pair<PackageSender*, NodeColor>(&r, NodeColor::UNVISITED));
        }
        for (auto &w : workers_list_) {
            color.insert(std::pair<PackageSender*, NodeColor>(&w, NodeColor::UNVISITED));
        }

        try {
            for (const auto& r: ramps_list_) {
                has_reachable_storehouse(&r, color);
            }
        }
        catch (std::logic_error& ex) {
            return false;
        }

        return true;
    }

void Factory::do_deliveries(Time t) {
    for(auto& ramp : ramps_) {
        ramp.deliver_goods(t);
    }
}

void Factory::do_package_passing() {

    for(auto& ramp : ramps_) {
        ramp.send_package();
    }

    for(auto& worker : workers_) {
        worker.send_package();
    }

}

void Factory::do_work(Time t) {
    for(auto& worker : workers_){
        worker.do_work(t);
    }
}