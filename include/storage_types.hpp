#ifndef LAB7_STORAGE_TYPES_HPP
#define LAB7_STORAGE_TYPES_HPP

#endif //LAB7_STORAGE_TYPES_HPP

class IPackageStockpile{
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual void push(Package&& aPackage) = 0;
    virtual bool empty() const = 0;
    virtual size_type size() const = 0;

    virtual std::list<Package>::iterator begin() = 0;
    virtual const_iterator cbegin() const = 0;
    virtual std::list<Package>::iterator end() = 0;
    virtual const_iterator cend() const = 0;

    virtual ~IPackageStockpile() {};
};