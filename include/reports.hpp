#ifndef NETSIM_REPORTS_HPP
#define NETSIM_REPORTS_HPP

#include "factory.hpp"
#include "nodes.hpp"
#include "types.hpp"

void generate_structure_report(const Factory& f, std::ostream& os);

void generate_simulation_turn_report(Factory f, std::ostream& os, Time T);

#endif //NETSIM_REPORTS_HPP
