#include "storage_types.hpp"

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
