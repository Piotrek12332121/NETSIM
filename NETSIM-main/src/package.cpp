#include "types.hpp"
#include "package.hpp"

#include <set>

// Inicjalizacja pól statycznych klasy Package
std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;

// Konstruktor domyślny klasy Package
Package::Package() {
    if(freed_IDs.empty()){
        if(assigned_IDs.empty()){
            id_ = 1;
        }
        else {
            id_ = *assigned_IDs.end() + 1;
        }
        assigned_IDs.insert(id_);
    }
    else{
        id_ = *freed_IDs.begin();
        freed_IDs.erase(freed_IDs.begin());
    }
}

// Przeciążony operator "=" klasy Package
Package& Package::operator= (Package&& diff_package) {
    assigned_IDs.erase(assigned_IDs.end());
    freed_IDs.insert(id_);
    id_ = diff_package.id_;
    return *this;
}