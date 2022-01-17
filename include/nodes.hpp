#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "types.hpp"
#include "helpers.hpp"
#include "storage_types.hpp"

#include <memory>
#include <optional>
#include <utility>


class IPackageReceiver{
public:
    virtual void receive_package(Package&& p) = 0;
    [[nodiscard]] virtual ElementID get_id() const = 0;
    virtual ~IPackageReceiver() = default;


    [[nodiscard]] virtual ReceiverType get_receiver_type() const = 0;
};


class Storehouse : public IPackageReceiver, public IPackageStockpile{
public:
    explicit Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d =
            std::make_unique<PackageQueue>(PackageQueueType::LIFO)) :
            id_(id), ptr_(std::move(d)) {};

    Storehouse(Storehouse&&) = default;

    void push(Package &&aPackage) override;
    bool empty() const override;
    size_t size() const override;

    iterator begin() override {return ptr_->begin();}
    [[nodiscard]] const_iterator cbegin() const override {return ptr_->cbegin();}
    iterator end() override {return ptr_->end();}
    [[nodiscard]] const_iterator cend() const override {return ptr_->cend();}

    void receive_package(Package&& p) override {ptr_->push(std::move(p));}
    [[nodiscard]] ElementID get_id() const override {return id_;}
    ~Storehouse() override = default;

    [[nodiscard]] ReceiverType get_receiver_type() const override {return ReceiverType::STOREHOUSE;}
private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> ptr_;
};


class ReceiverPreferences {
public:
    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : pg_(std::move(pg)) {}

    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    void add_receiver(IPackageReceiver* r);

    void remove_receiver(IPackageReceiver* r);

    IPackageReceiver* choose_receiver();

    [[nodiscard]] const preferences_t& get_preferences() const {return preferences_;}

    [[nodiscard]] const_iterator begin() const {return preferences_.begin();}
    [[nodiscard]] const_iterator cbegin() const {return preferences_.cbegin();}
    [[nodiscard]] const_iterator end() const {return preferences_.end();}
    [[nodiscard]] const_iterator cend() const {return preferences_.cend();}
private:
    ProbabilityGenerator pg_;
    double n = 0.0;
    preferences_t preferences_;
};


class PackageSender   //TODO: do poprawy wysypuje std::optional
{
public:
    //PackageSender(PackageSender&&)=default;

    void send_package();

    [[nodiscard]] std::optional<Package> get_sending_buffer() const {return bucket;}

    ReceiverPreferences receiver_preferences_;

protected:
    void push_package(Package &&package) { bucket = std::move(package); }

private:
    std::optional<Package> bucket;
};


class Worker : public PackageSender, public IPackageReceiver, public IPackageQueue{
public:
    using queue_t = std::unique_ptr<PackageQueue>;

    Worker(ElementID id, TimeOffset pd, std::unique_ptr<PackageQueue> q);

    void push(Package&& aPackage) override;
    bool empty() const override;
    size_t size() const override;
    PackageQueueType get_queue_type() const override;
    Package pop() override;

    void do_work(Time t);

    void receive_package(Package&& p) override { queue->push(std::move(p)); }

    [[nodiscard]] ReceiverType get_receiver_type() const override{ return ReceiverType::WORKER; }

    [[nodiscard]] TimeOffset get_processing_duration() const { return pd_; }

    [[nodiscard]] Time get_package_processing_start_time() const { return starttime; }

    [[nodiscard]] ElementID get_id() const override { return id_;}

    iterator begin() override { return queue->begin(); };
    iterator end() override { return queue->end();};
    [[nodiscard]] const_iterator cbegin() const override { return queue->cbegin();};
    [[nodiscard]] const_iterator cend() const override { return queue->cend();};

    PackageQueue* get_queue(void) const
    {
        return queue.get();
    }

    std::optional<Package> get_processing_buffer() const
    {
        return processing
    }

private:
    ElementID id_;
    TimeOffset pd_;
    queue_t queue;
    Time starttime;
    std::optional<Package> processing;
};


class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {}
    [[nodiscard]] TimeOffset get_delivery_interval() const {return di_;} // Getter offsetu
    [[nodiscard]] ElementID get_id() const {return id_;} // Getter ID
    void deliver_goods(Time t) { if(t % di_ == 0) { send_package(); } }
private:
    ElementID id_; // ID rampy
    TimeOffset di_; // Czas wykonania pojedyńczej wysyłki
};

#endif //NETSIM_NODES_HPP
