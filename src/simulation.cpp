#include "simulation.hpp"

void simulate(Factory& f, TimeOffset d, const std::function<void (Factory&, Time)>& rf){
    if (f.is_consistent()) {
        for (Time i = 1; i <= d; i++) {
            f.do_deliveries(i);
            f.do_package_passing();
            f.do_work(i);
            rf(f, i);
        }
    }
    else {
        throw std::logic_error("Fabryka nie jest konsytentna");
    }
}