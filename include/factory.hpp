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
    void add_ramp(Ramp&&);
    void remove_ramp(ElementID id);
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return ramps_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return ramps_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cbegin() const {return ramps_.begin();}
    [[nodiscard]] NodeCollection<Ramp>::const_iterator ramp_cend() const {return ramps_.cend();}

    void add_worker(Worker&&);
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) {return workers_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const {return workers_.find_by_id(id);}
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cbegin() const {return workers_.begin();}
    [[nodiscard]] NodeCollection<Worker>::const_iterator worker_cend() const {return workers_.begin();}

    void add_storehouse(Storehouse&&);
    void remove_storehouse(ElementID id);
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


Factory load_factory_structure(std::istream& is){
    Factory factory;
    std::string line;

    while (std::getline(is, line)){
        if (line[0] == ';' or line.empty()){
            continue;
        }
        ParsedLineData parsed_line = parse_line(line);
        if (parsed_line.element_type == ElementType::LOADING_RAMP) {
            Ramp r = Ramp(std::stoi(parsed_line.parameters["id"]),
                          std::stoul(parsed_line.parameters["delivery-interval"]));
            factory.add_ramp(std::move(r));               
        }
        else if (parsed_line.element_type == ElementType::WORKER) {
            PackageQueueType queue_type;
            if (parsed_line.parameters["queue-type"] == "FIFO") {
                queue_type = PackageQueueType::FIFO;
            } else if (parsed_line.parameters["queue-type"] == "LIFO") {
                queue_type = PackageQueueType::LIFO;
            }
            Worker w = Worker(std::stoi(parsed_line.parameters["id"]),
                              std::stoul(parsed_line.parameters["processing-time"]),
                              std::make_unique<PackageQueue>(queue_type));
            factory.add_worker(std::move(w));
        }
        else if (parsed_line.element_type == ElementType::STOREHOUSE) {
            Storehouse s = Storehouse(std::stoi(parsed_line.parameters["id"]));
            factory.add_storehouse(std::move(s));
        }
        else if (parsed_line.element_type == ElementType::LINK){
            std::string source_pair = parsed_line.parameters["src"];
            std::string source_type;
            std::string source_id;
            for (std::size_t i = 0; i < source_pair.size(); ++i) {
                if (source_pair[i] == '-') {
                    source_type = source_pair.substr(0, i);
                    source_id = source_pair.substr(i + 1, source_pair.size() - i - 1);
                }
            }
            std::string destination_pair = parsed_line.parameters["dest"];
            std::string receiver_type;
            std::string receiver_id;
            for (std::size_t i = 0; i < destination_pair.size(); ++i) {
                if (destination_pair[i] == '-') {
                    receiver_type = destination.pair.substr(0, i);
                    receiver_id = destination_pair.substr(i + 1, destination_pair.size() - i - 1);
                }
            }
            if (source_type == "ramp" && receiver_type == 'worker') {
                Ramp& r - *(factory.find_ramp_by_id(std::stoi(source_id)));
                r.receiver_preferences_.add_receiver(&(*(factory.find_worker_by_id(std::stoi(receiver_id)))));
            }
            else if (source_type == "ramp" && receiver_type == 'store') {
                Ramp& r - *(factory.find_ramp_by_id(std::stoi(source_id)));
                r.receiver_preferences_.add_receiver(&(*(factory.find_storehouse_by_id(std::stoi(receiver_id)))));
            }
            else if (source_type == "worker" && receiver_type == 'worker') {
                Worker& w - *(factory.find_worker_by_id(std::stoi(source_id)));
                r.receiver_preferences_.add_receiver(&(*(factory.find_worker_by_id(std::stoi(receiver_id)))));
            }
            else if (source_type == "worker" && receiver_type == 'store') {
                Worker& w - *(factory.find_worker_by_id(std::stoi(source_id)));
                r.receiver_preferences_.add_receiver(&(*(factory.find_storehouse_by_id(std::stoi(receiver_id)))));
            }
        }
    }
    return factory;
}

void save_factory_structure(Factory& factory, std::ostream& os) {
    os << "; --- LOADING RAMPS ---" << std::endl;
    os << std::endl;
    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) {
        os << "LOADING_RAMP id=" << it->get_id() << " delivery-interval=" << it->get_delivery_interval() << std::endl;
    }

    os << std::endl;
    os << ": --- WORKER ---" << std::endl;
    os << std::endl;
    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) {
        std::string queue_type;
        if (it->get_queue()->get_queue_type() == PackageQueueType::FIFO) {
            queue_type = "FIFO";
        }
        else if (it->get_queue()->get_queue_type() == PackageQueueType::LIFO) {
            queue_type = "LIFO";
        }
        os << "Worker id=" << it->get_id() << "processing-time=" << it->get_processing_duration() << " queue-type" << queue_type << std::endl;
    }

    os << std::endl;
    os << "; --- STOREHOUSE ---" << std::endl;
    os << std::endl;
    for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it) {
        os << "STOREHOUSE id=" << it->get_id() << std::endl;
    }

    os << std::endl;
    os << "; --- LINKS ---" << std::endl;
    os << std::endl;
    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) {
        make_connection(*it, os);
    }

    for(auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) {
        make_connection(*it, os);
    }
    os.flush();
}

#endif //NETSIM_FACTORY_HPP
