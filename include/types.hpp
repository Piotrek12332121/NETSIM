#ifndef LAB7_TYPES_HPP
#define LAB7_TYPES_HPP

#include "storage_types.hpp"
#include <functional>
#include <map>

using ElementID = int;

using TimeOffset = int;
using Time = int;
using ProbabilityGenerator = std::function<double(void)>;

enum class ReceiverType{
    WORKER = 0,
    STOREHOUSE = 1
};

#endif //LAB7_TYPES_HPP
