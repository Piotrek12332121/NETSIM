#include "nodes.hpp"
<<<<<<< HEAD

=======
#include "helpers.hpp"


void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    double old_ratio = n;
    n += 1.0;
    preferences_t receiver_to_add{ {std::move(r), 1.0/n} };

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



const preferences_t& ReceiverPreferences::get_preferences() const{
    return preferences_;
}
>>>>>>> 09ae921aab40367b92bf2f0929f9dfe3e67567e2
