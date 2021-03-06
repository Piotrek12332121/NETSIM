#ifndef LAB7_PACKAGE_HPP
#define LAB7_PACKAGE_HPP

#include "types.hpp"

#include <set>

class Package{
    using ElementID = int;
public:
    Package();  // Konstruktor domyślny
    explicit Package(const ElementID& id) :  id_(id) {} // Konstruktor na podstawie ID do raportów
    Package(const Package&& diff_package)  noexcept : id_(diff_package.id_) {}    // Konstruktor kopiujący

    Package& operator= (Package&& diff_package) noexcept; // Przeciążony operator "="

    ElementID get_id() const { return id_; }  // Getter ID

    ~Package() { freed_IDs.insert(id_); } // Destruktor klasy
private:
    static std::set<ElementID> assigned_IDs;    // Współdzielone przez każdy obiekt
    static std::set<ElementID> freed_IDs;  // Kontener usuniętych ID do ponownego użycia
    ElementID id_;  // ID charakterystyczne dla obiektu
};

#endif //LAB7_PACKAGE_HPP
