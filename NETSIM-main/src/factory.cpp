#XXX


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