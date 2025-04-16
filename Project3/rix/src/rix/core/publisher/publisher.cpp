#include "rix/core/publisher/publisher.hpp"

namespace rix {
namespace core {

Publisher::~Publisher() {
    // TODO: Implement the destructor. The destructor should:
    //       1. Call the shutdown method.
}

std::string Publisher::get_topic() const { 
    // TODO: Implement the get_topic method. This method should:
    //       1. Return the topic of the Publisher.

    return "";
}

size_t Publisher::get_num_subscribers() const { 
    // TODO: Implement the get_num_subscribers method. The method should:
    //       1. Return the number of subscribers in a thread-safe manner. (use 
    //          the get_num_subscribers method of the implementation).

    return 0;
}

void Publisher::shutdown() { 
    // TODO: Implement the shutdown method. The method should:
    //       1. Set the request_shutdown_flag to true. This will notify the Node
    //          to stop the Publisher. Once the Node's thread has checked this 
    //          publisher's flag, it will call the implementation's shutdown 
    //          method, which will stop the thread.
    //       2. If the calling thread is not the thread running the Publisher
    //          and the thread is joinable, call join on the thread to wait for 
    //          it to finish.
}

bool Publisher::ok() const {
    return impl->ok(); 
}

Publisher::Publisher(const std::string &topic, uint64_t node_id, const rix::msg::component::MessageInfo &msg_info,
                     std::shared_ptr<PubImplBase> impl) {
    // TODO: Implement the Publisher constructor. The constructor should:
    //       1. Initialize the member variables of the Publisher.
    //       2. Initialize the fields of the ComponentInfo structure.

}

void Publisher::add_subscriber(const rix::msg::component::ID &id) {
    // TODO: Implement the add_subscriber method. This method should:
    //       1. Add the ID to the accept_sub_container in a thread-safe manner.

}

void Publisher::remove_subscriber(const rix::msg::component::ID &id) {
    // TODO: Implement the remove_subscriber method. This method should:
    //       1. Add the ID to the remove_sub_container in a thread-safe manner.

}

rix::msg::component::ID Publisher::get_id() const {
    // TODO: Implement the get_id method. This method should:
    //       1. Return the ID of the Publisher. The ID should contain the
    //          component ID and the URI of the Publisher. Use the get_uri 
    //          method of the implementation to get the URI of the Publisher.

    return rix::msg::component::ID();
}

void Publisher::start() {
    // TODO: Implement the start method. This method should:
    //       1. Invoke the run method of the Publisher in a new thread.
}

void Publisher::run() {
    Logger::log<LogLevel::DEBUG>("Publisher::run called.");
    thread_id = std::this_thread::get_id();
    while (ok()) {
        run_once();
    }
    Logger::log<LogLevel::DEBUG>("Publisher::run returning.");
}

void Publisher::run_once() {
    // TODO: Implement the run_once method. This method should:
    //       1. Lock the mutex.
    //       2. If the accept_sub_container is not empty, call the accept_connections
    //          method of the implementation with the accept_sub_container.
    //       3. If the remove_sub_container is not empty, call the remove_connections
    //          method of the implementation with the remove_sub_container.
    //       4. Unlock the mutex.
}

}  // namespace core
}  // namespace rix