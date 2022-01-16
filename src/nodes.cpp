#include "nodes.hpp"
#include "helpers.hpp"

#include <algorithm>
#include <memory>
#include <optional>

void PackageSender::send_package(){
    if(bucket){
        IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
        receiver->receive_package(std::move(*bucket));
        bucket.reset();
    }
}

void Worker::do_work(Time t) {

    if (!queue->empty() && !processing) {
        processing = queue->pop();
        starttime = t;
    }

    if (t - starttime == pd_ - 1 && processing) {
        push_package(std::move(*processing));
        processing.reset();
    }
}

Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<PackageQueue> q) {
    id_ = id;
    pd_ = pd;
    queue = std::move(q);
    starttime = 0;
    processing = std::nullopt;
}

void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    double old_ratio = n;
    n += 1.0;
    preferences_t receiver_to_add{ {r, 1.0/n} };

    if (!preferences_.empty()) {
        double factor_scale = old_ratio / n;
        for (auto& it: preferences_) {
            it.second *= factor_scale;
        }
    }
    preferences_.merge(receiver_to_add);

}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    double old_ratio = n;
    auto searched_iter = preferences_.find(r);
    preferences_.erase(searched_iter);
    n -= 1.0;
    double factor_scale = old_ratio / n;
    for (auto& it: preferences_) {
        it.second *= factor_scale;
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    double sum = 0.0;
    auto pg_temp = pg_();

    for (const auto& x : preferences_) {
        sum += x.second;
        if (sum >= pg_temp) {
            return x.first;
        }
    }
    return nullptr;
}
