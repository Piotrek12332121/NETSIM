#include "factory.hpp"
#include "nodes.hpp"

#include <stdexcept>

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
        } else {
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
}
