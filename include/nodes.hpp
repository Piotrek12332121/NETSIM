#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "types.hpp"
#include "helpers.hpp"

#include <memory>

enum class ReceiverType{
    WORKER = 0,
    STOREHOUSE = 1
};


class IPackageReceiver{
public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;
    virtual ~IPackageReceiver() = default;


    virtual ReceiverType get_receiver_type() const = 0;
};



class Storehouse : public IPackageReceiver, public IPackageStockpile{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d =
            std::make_unique<PackageQueue>(PackageQueueType::LIFO)) :
            id_(id), ptr_(std::move(d)) {};

    Storehouse(Storehouse&&) = default;

    IPackageStockpile::iterator begin() override {return ptr_->begin();}
    IPackageStockpile::const_iterator cbegin() const override {return ptr_->cbegin();}
    IPackageStockpile::iterator end() override {return ptr_->end();}
    IPackageStockpile::const_iterator cend() const override {return ptr_->cend();}

    void receive_package(Package&& p) override {ptr_->push(std::move(p));}
    ElementID get_id() const override {return id_;}
    ~Storehouse() override = default;


    ReceiverType get_receiver_type() const override {return ReceiverType::STOREHOUSE;}
private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> ptr_;
};


class ReceiverPreferences {
public:
    ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : pg_(pg) {}

    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    void add_receiver(IPackageReceiver* r);

    void remove_receiver(IPackageReceiver* r);

    IPackageReceiver* choose_receiver();

    const preferences_t& get_preferences() const;

    const_iterator begin() const {return preferences_.begin();}
    const_iterator cbegin() const {return preferences_.cbegin();}
    const_iterator end() const {return preferences_.end();}
    const_iterator cend() const {return preferences_.cend();}
private:
    ProbabilityGenerator pg_;

    double n = 0.0;

    preferences_t preferences_;
};

class PackageSender   //TODO: do poprawy wysypuje std::optional
{
public:
    //PackageSender(PackageSender&&)=default;
    void send_package()
    {
        if(bucket)
        {
            IPackageReceiver* receiver =receiver_preferences_.choose_receiver();
            receiver->receive_package(std::move(*bucket));
            bucket.reset();
        }
    }
    std::optional<Package> get_sending_buffer()
    {
        return bucket;
    }
    bool buffer_empty()
    {
        if(bucket) return true;
        else return false;
    }
    ReceiverPreferences receiver_preferences_;
protected:
    void push_package(Package &&package)
    {
        bucket=std::move(package);
    }

private:
    std::optional<Package> bucket;
};


class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {}
    TimeOffset get_delivery_interval() const {return di_;} // Getter offsetu
    ElementID get_id() const {return id_;} // Getter ID
    void deliver_goods(Time t) const { if(t % di_ == 0) { send_package(); } }
private:
    ElementID id_; // ID rampy
    TimeOffset di_; // Czas wykonania pojedyńczej wysyłki
};


class Worker : public PackageSender, public IPackageReceiver
{
public:
    using queue_t=std::unique_ptr<PackageQueue>;

    Worker(ElementID id, TimeOffset pd, std::unique_ptr<PackageQueue> q) //TODO:ogarnąc pola składowe
    {
        id_=id;
        pd_=pd;
        queue=std::move(q);
        starttime=0;
        processing=std::nullopt;

    }
    void do_work(Time t)
    {

        if(!queue->empty() && !processing)
        {
            processing=queue->pop();
            starttime=t;
        }

        if (t-starttime==pd_-1  && processing)
        {
            push_package(std::move(*processing));
            processing.reset();
        }
        void receive_package(Package &&p)
        {

            queue->push(std::move(p));
        }

    // TODO: ReceiverType get_receiver_type(){ return ReceiverType::WORKER; };
        TimeOffset Worker::get_processing_duration()
        {
            return pd_;
        }
        Time get_package_processing_start_time()
        {
            return starttime;
        }
        ElementID Worker::get_id() const
        {
            return id_;
        }

    const_iterator begin() const { return queue->cbegin(); };
    const_iterator end() const { return queue->cend();};
    const_iterator cbegin() const { return queue->cbegin();};
    const_iterator cend() const { return queue->cend();};

private:
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<PackageQueue> queue;
    Time starttime;
    std::optional<Package> processing;
};

#endif //NETSIM_NODES_HPP
