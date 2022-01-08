#include "storage_types.hpp"

void PackageQueue::push(Package&& other){
    queue_.emplace_back(std::move(other));
}

bool PackageQueue::empty() {
    return queue_.empty();
}

std::size_t PackageQueue::size(){
    return queue_.size();
}

Package PackageQueue::pop() {
    switch(queue_type_) {
        case PackageQueueType::LIFO: {
            Package popped_element_back = std::move(queue_.back());
            queue_.pop_back();
            return popped_element_back;
        }
        case PackageQueueType::FIFO: {
            Package popped_element_front = std::move(queue_.front());
            queue_.pop_front();
            return popped_element_front;
        }
        default: throw;
    }

}

PackageQueueType PackageQueue::get_queue_type() {
    return queue_type_;
}
