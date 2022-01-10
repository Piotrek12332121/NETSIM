#ifndef LAB7_TYPES_HPP
#define LAB7_TYPES_HPP

#import "storage_types.hpp"

#import <map>

using ElementID = int;

using TimeOffset = int;
using Time = int;

using preferences_t = std::map<IPackageReceiver*, double>;
using const_iterator = preferences_t::const_iterator;

#endif //LAB7_TYPES_HPP
