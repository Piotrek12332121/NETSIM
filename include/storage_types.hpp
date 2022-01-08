#ifndef LAB7_STORAGE_TYPES_HPP
#define LAB7_STORAGE_TYPES_HPP

#include "types.hpp"
#include <list>
#include "package.hpp"


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

class PackageQueue : public IPackageQueue {
public:
    PackageQueue(PackageQueueType queue_type) : queue_type_(queue_type) {}
    void push(Package&& other) override;
    bool empty() override;
    std::size_t size() override;
    Package pop() override;
    PackageQueueType get_queue_type() override;

    const_iterator begin() const override {return queue_.begin();}
    const_iterator cbegin() const override {return queue_.cbegin();}
    const_iterator end() const override {return queue_.end();}
    const_iterator cend() const override {return queue_.cend();}

private:
    std::list<Package> queue_;
    PackageQueueType queue_type_;
};

#endif //LAB7_STORAGE_TYPES_HPP