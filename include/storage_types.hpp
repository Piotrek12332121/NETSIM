#ifndef LAB7_STORAGE_TYPES_HPP
#define LAB7_STORAGE_TYPES_HPP

#include "types.hpp"
#include "package.hpp"

#include <deque>
#include <functional>
#include <iostream>
#include <list>

enum class PackageQueueType {
    FIFO, LIFO
};

class IPackageStockpile{
public:
    using const_iterator = std::list<Package>::const_iterator;
    using iterator = std::list<Package>::iterator;

    virtual void push(Package&& aPackage) = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;

    virtual iterator begin() = 0;
    virtual const_iterator cbegin() const = 0;
    virtual iterator end() = 0;
    virtual const_iterator cend() const = 0;

    virtual ~IPackageStockpile() = default;
};


class IPackageQueue : public IPackageStockpile {
public:
    virtual PackageQueueType get_queue_type() const = 0;
    virtual Package pop() = 0;
private:

};


class PackageQueue : public IPackageQueue {
public:
    explicit PackageQueue(PackageQueueType queue_type) : queue_type_(queue_type) {}
    void push(Package&& other) override {queue_.emplace_back(std::move(other));}
    bool empty() const override {return queue_.empty();}
    std::size_t size() const override {return queue_.size();}
    Package pop() override;
    PackageQueueType get_queue_type() const override {return queue_type_;}

    iterator begin() override {return queue_.begin();}
    const_iterator cbegin() const override {return queue_.cbegin();}
    iterator end() override {return queue_.end();}
    const_iterator cend() const override {return queue_.cend();}

private:
    std::list<Package> queue_;
    PackageQueueType queue_type_;
};

#endif //LAB7_STORAGE_TYPES_HPP