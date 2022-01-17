#include "factory.hpp"
#include "nodes.hpp"

#include <stdexcept>

bool Factory::has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) const {
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

bool Factory::is_consistent() const {
    std::map<const PackageSender*, NodeColor> color;

    for (auto &r : ramps_) {
        color.insert(std::pair<PackageSender*, NodeColor>(&r, NodeColor::UNVISITED));
    }
    for (auto &w : workers_) {
        color.insert(std::pair<PackageSender*, NodeColor>(&w, NodeColor::UNVISITED));
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
    for(auto& worker: workers_){
        worker.do_work(t);
    }
}

ParsedLineData parse_line (const std::string& line) {
    std::vector<std::string> tokens;
    std::string token;

    std::istringstream token_stream(line);
    char delimiter = ' ';

    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }

    std::map<std::string, ElementType> types = {{"LOADING_RAMP",ElementType::LOADING_RAMP}, {"WORKER",ElementType::WORKER},
                                                {"STOREHOUSE",ElementType::STOREHOUSE}, {"LINK",ElementType::LINK}};

    ElementType type;
    bool is_suitable = false;
    for (auto& x : types) {
        if (tokens[0] == x.first) {
            type = x.second;
            is_suitable = true;
        }
    }

    if (!is_suitable) {
        throw std::logic_error("Not suitable type");
    }

    ParsedLineData parsed_line;
    parsed_line.element_type = type;

    tokens.erase(tokens.begin());
    std::pair<std::string, std::string> pair;
    for (auto& i : tokens) {
        for (std::size_t j = 0; j < i.size(); ++j) {
            if (i[j] == '=') {
                pair.first = i.substr(0, j);
                pair.second = i.substr(j + 1, i.size() - j - 1);
            }
            parsed_line.parameters.insert(pair);
        }
    }

    return parsed_line;
}

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
                Ramp& r = *(factory.find_ramp_by_id(std::stoi(source_id)));
                r.receiver_preferences_.add_receiver(&(*(factory.find_worker_by_id(std::stoi(receiver_id)))));
            }
            else if (source_type == "ramp" && receiver_type == 'store') {
                Ramp& r = *(factory.find_ramp_by_id(std::stoi(source_id)));
                r.receiver_preferences_.add_receiver(&(*(factory.find_storehouse_by_id(std::stoi(receiver_id)))));
            }
            else if (source_type == "worker" && receiver_type == 'worker') {
                Worker& w = *(factory.find_worker_by_id(std::stoi(source_id)));
                r.receiver_preferences_.add_receiver(&(*(factory.find_worker_by_id(std::stoi(receiver_id)))));
            }
            else if (source_type == "worker" && receiver_type == 'store') {
                Worker& w = *(factory.find_worker_by_id(std::stoi(source_id)));
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
