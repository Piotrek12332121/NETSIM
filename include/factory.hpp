#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP

#include "types.hpp"

#include <vector>
#include <algorithm>

template <class Node>
class NodeCollection{
    using container_t = typename std::vector<Node>; // Wybranym kontenerem jest std::vector
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;
public:
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

class Factory{
public:
    bool is_consistent();

    void do_deliveries(Time t);

    void do_package_passing();

    void do_work(Time t);

private:
    NodeCollection <Worker> workers_;
    NodeCollection <Storehouse> storehouses_;
    NodeCollection <Ramp> ramps_;

    template<class Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id) {collection.remove_by_id(id);}

};

#endif //NETSIM_FACTORY_HPP
