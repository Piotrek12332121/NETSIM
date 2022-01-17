#ifndef NETSIM_SIMULATION_HPP
#define NETSIM_SIMULATION_HPP


#include "reports.hpp"
#include "factory.hpp"
#include "types.hpp"
#include <functional>

void simulate(Factory& f, TimeOffset d, const std::function<void (Factory&, Time)>& rf);
#endif //NETSIM_SIMULATION_HPP

