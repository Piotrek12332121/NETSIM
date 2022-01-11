#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP

#include "types.hpp"
#include "nodes.hpp"

#include <vector>
#include <algorithm>

enum class NodeColor { UNVISITED, VISITED, VERIFIED };

template <class Node>
class NodeCollection{
public:
    using container_t = typename std::vector<Node>; // Wybranym kontenerem jest std::vector
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node&& node) {
        collection_.push_back(std::move(node));
    }

    NodeCollection<Node>::iterator find_by_id(ElementID id) { return std::find_if(collection_.begin(), collection_.end(),
                                                                                        [id] (Node node) { return node.id_ == id;} ); };
    NodeCollection<Node>::const_iterator find_by_id(ElementID id) const { return std::find_if(collection_.begin(), collection_.end(),
                                                                                              [id] (Node node) { return node.id_ == id;} ); };

    void remove_by_id(ElementID id) {
        NodeCollection<Node>::iterator target_elem;
        target_elem = find_by_id(id);
        collection_.erase(collection_.end(), target_elem);
    }

    typename std::vector<Node>::const_iterator cbegin() const { return collection_.cbegin(); }
    typename std::vector<Node>::const_iterator cend() const { return collection_.cend(); }
    typename std::vector<Node>::iterator begin() { return collection_.begin(); }
    typename std::vector<Node>::const_iterator begin() const { return collection_.cbegin(); }
    typename std::vector<Node>::iterator end() { return collection_.end(); }
    typename std::vector<Node>::const_iterator end() const { return collection_.cend(); }
private:
    container_t collection_;
};


template <class Node>
class Factory{
public:
    bool is_consistent() {
        std::map<const PackageSender*, NodeColor> color;

        for (auto &r : ramps_) {
            color.insert(std::pair<*PackageSender, NodeColor>(&r, NodeColor::UNVISITED));
        }
        for (auto &w : workers_) {
            color.insert(std::pair<*PackageSender, NodeColor>(&w, NodeColor::UNVISITED));
        }

        try {
            for (const auto& r: ramps_) {
                has_reachable_storehouse(&r, color);
            }
        }
        catch (std::logic_error& ex) {
            return false;
        }

        return true;
    }

    void do_deliveries(Time t) {
        for(auto& ramp : ramps_) {
            ramp.deliver_goods(t);
        }
    }

    void do_package_passing() {
        for(auto& ramp : ramps_) {
            ramp.send_package();
        }

        for(auto& worker : workers_) {
            worker.send_package();
        }
    }

    void do_work(Time t);
private:
    NodeCollection <Worker> workers_;
    NodeCollection <Storehouse> storehouses_;
    NodeCollection <Ramp> ramps_;

    void remove_receiver(NodeCollection<Node>& collection, ElementID id) {collection.remove_by_id(id);}
};

#endif //NETSIM_FACTORY_HPP
