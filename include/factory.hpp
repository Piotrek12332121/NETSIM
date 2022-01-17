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

    void add(Node&& node) {collection_.push_back(std::move(node));}

    NodeCollection<Node>::iterator find_by_id(ElementID id) { return std::find_if(collection_.begin(), collection_.end(),
                                                                                        [id] (Node node) { return node.get_id() == id;} ); };
    [[nodiscard]] NodeCollection<Node>::const_iterator find_by_id(ElementID id) const { return std::find_if(collection_.begin(), collection_.end(),
                                                                                              [id] (Node node) { return node.get_id() == id;} ); };

    void remove_by_id(ElementID id) {
        NodeCollection<Node>::iterator target_elem;
        target_elem = find_by_id(id);
        collection_.erase(collection_.end(), target_elem);
    }

    [[nodiscard]] typename std::vector<Node>::const_iterator cbegin() const { return collection_.cbegin(); }
    [[nodiscard]] typename std::vector<Node>::const_iterator cend() const { return collection_.cend(); }
    [[nodiscard]] typename std::vector<Node>::const_iterator begin() const { return collection_.cbegin(); }
    [[nodiscard]] typename std::vector<Node>::const_iterator end() const { return collection_.cend(); }
    typename std::vector<Node>::iterator begin() { return collection_.begin(); }
    typename std::vector<Node>::iterator end() { return collection_.end(); }
private:
    container_t collection_;
};


class Factory{
public:
    void add_ramp(Ramp&& r) {ramps_.add(std::move(r));}
    void remove_ramp(ElementID id) {ramps_.remove_by_id(id);}
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return ramps_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return ramps_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cbegin() const {return ramps_.begin();}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cend() const {return ramps_.cend();}

    void add_worker(Worker&& w) { workers_.add(std::move(w)); }
    void remove_worker(ElementID id) {workers_.remove_by_id(id);}
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) {return workers_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const {return workers_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cbegin() const {return workers_.begin();}
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cend() const {return workers_.begin();}

    void add_storehouse(Storehouse&& s) {storehouses_.add(std::move(s));}
    void remove_storehouse(ElementID id) {storehouses_.remove_by_id(id);}
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {return storehouses_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const {return storehouses_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const {return storehouses_.begin();}
    [[nodiscard]] NodeCollection<Storehouse>::const_iterator storehouse_cend() const {return storehouses_.begin();}

    bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) const;

    [[nodiscard]] bool is_consistent() const;

    void do_deliveries(Time t);

    void do_package_passing();

    void do_work(Time t);
private:
    NodeCollection <Ramp> ramps_;
    NodeCollection <Worker> workers_;
    NodeCollection <Storehouse> storehouses_;

    template<class Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id) {collection.remove_by_id(id);}
};


Factory load_factory_structure(std::istream& is);

void save_factory_structure(Factory& factory, std::ostream& os);

#endif //NETSIM_FACTORY_HPP
