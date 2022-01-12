#ifndef LAB7_TYPES_HPP
#define LAB7_TYPES_HPP

#include "storage_types.hpp"
#include <map>

using ElementID = int;

using TimeOffset = int;
using Time = int;
using ProbabilityGenerator = std::function<double(void)>;

#endif //LAB7_TYPES_HPP
