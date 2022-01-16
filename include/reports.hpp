#ifndef NETSIM_REPORTS_HPP
#define NETSIM_REPORTS_HPP

#include <utility>

#include "factory.hpp"
#include "nodes.hpp"
#include "types.hpp"

void generate_structure_report(const Factory& f, std::ostream& os);

void generate_simulation_turn_report(Factory f, std::ostream& os, Time T);

class IntervalReportNotifier{
public:
    explicit IntervalReportNotifier(TimeOffset to) : to_(to) {}
    [[nodiscard]] bool should_generate_report(Time t) const;
private:
    TimeOffset to_;
};


class SpecificTurnsReportNotifier{
public:
    explicit SpecificTurnsReportNotifier(std::set<Time> turns) : turns_(std::move(turns)) {}
    bool should_generate_report(Time T);
private:
    std::set<Time> turns_;
};
#endif //NETSIM_REPORTS_HPP
