#include "reports.hpp"

#include <ostream>
#include <map>
#include <iterator>

void generate_structure_report(const Factory& f, std::ostream& os){
    if(f.is_consistent()) {
        //Raport ramp załadunkowych
        os << "\n==LOADING RAMPS==\n" << std::endl;

        for (auto ramp = f.ramp_cbegin(); ramp != f.ramp_cend(); ++ramp){
            os << "LOADING RAMP #" << ramp->get_id() << std::endl;
            os << "  Delivery interval: " << ramp->get_delivery_interval() << std::endl;
            os << "  Receivers:" << std::endl;
            for (const auto& [key, value] : ramp->receiver_preferences_.get_preferences()){
                os << "    " << static_cast<std::underlying_type<ReceiverType>::type>(key->get_receiver_type()) << " #" << key->get_id() << std::endl;
            }
        }

        //Raport pracowników
        os << "\n==WORKERS==\n" << std::endl;

        for (auto worker = f.worker_cbegin(); worker != f.worker_cend(); ++worker){
            os << "WORKER #" << worker->get_id() << std::endl;
            os << "  Procesing time: " << worker->get_processing_duration() << std::endl;
            os << "  Queue type: " << worker.get_queue().get_queue_type() << std::endl;
            os << "  Receivers:" << std::endl;
            for (const auto& [key, value] : worker->receiver_preferences_.get_preferences()){
                os << "    " << static_cast<std::underlying_type<ReceiverType>::type>(key->get_receiver_type()) << " #" << key->get_id() << std::endl;
            }
        }

        //Raport magazynów
        os << "\n==STOREHOUSES==\n" << std::endl;

        for (auto storehouse = f.storehouse_cbegin(); storehouse != f.storehouse_cend(); ++storehouse){
            os << "STOREHOUSE #" << storehouse->get_id() << std::endl;
        }
    }
}

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time T){
    if(f.is_consistent()){
        //Raport tury
        os << "=== [ Turn: " << T << " ] ===" << std::endl;

        //Raport pracowników
        os << "\n==WORKERS==\n" << std::endl;

        for (auto worker = f.worker_cbegin(); worker != f.worker_cend(); ++worker){
            os << "WORKER #" << worker->get_id() << std::endl;

            if(worker->get_processing_buffer()) {
                os << "  PBuffer: #" << worker->get_processing_buffer().get_id() << " (pt = " << T - worker->get_package_processing_start_time() << ")"<< std::endl;
            }
            else{
                os << "  PBuffer: (empty)" << std::endl;
            }

            if(worker->cbegin() != worker->cend()){
                os << "Queue: #";
                for(auto package = worker->cbegin(); package != worker->cend(); ++package){
                    if(package != std::prev(worker->cend())){
                        os << package->get_id() << ", #";
                    }
                    else{
                        os << package->get_id() << std::endl;
                    }
                }
            }
            else{
                os << "Queue: #";
            }

            if(worker->get_sending_buffer()){
                os << "SBuffer: #" << worker->get_sending_buffer()->get_id() << std::endl;
            }
            else{
                os << "SBuffer: (empty)" << std::endl;
            }
        }

        //Raport magazynów
        os << "\n==STOREHOUSES==\n" << std::endl;

        for (auto storehouse = f.storehouse_cbegin(); storehouse != f.storehouse_cend(); ++storehouse){
            os << "STOREHOUSE #" << storehouse->get_id() << std::endl;
            if(storehouse->cbegin() != storehouse->cend()){
                os << "Stock: #";
                for(auto package = storehouse->cbegin(); package != storehouse->cend(); ++package){
                    if(package != std::prev(storehouse->cend())){
                        os << package->get_id() << ", #";
                    }
                    else{
                        os << package->get_id() << std::endl;
                    }
                }
            }
        }
    }
}

bool IntervalReportNotifier::should_generate_report(Time t) const{
    if( t % to_ == 0 ){
        return true;
    }
    else{
        return false;
    }
}

bool SpecificTurnsReportNotifier::should_generate_report(Time T){
    for (const auto& turn : turns_){
        if (T == turn){
            return true;
        }
        else{
            return false;
        }
    }
}