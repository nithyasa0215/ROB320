#include "rix/core/subscriber/subscriber.hpp"

namespace rix {
namespace core {

Subscriber::~Subscriber() {
    // TODO: Implement the destructor. The destructor should:
    //       1. Call the shutdown method.
}

std::string Subscriber::get_topic() const { 
    // TODO: Implement the get_topic method. This method should:
    //       1. Return the topic of the Subscriber.

    return "";
}

int Subscriber::get_num_publishers() const { 
    // TODO: Implement the get_num_subscribers method. This method should:
    //       1. Return the number of publishers in a thread-safe manner (use the
    //          get_num_publishers method of the implementation).
    
    return 0;
}

void Subscriber::shutdown() { 
    // TODO: Implement the shutdown method. The method should:
    //       1. Set the request_shutdown_flag to true. This will notify the Node
    //          to stop the Subscriber. Once the Node's thread has checked this 
    //          subscriber's flag, it will call the implementation's shutdown 
    //          method, which will stop the thread.
    //       2. If the calling thread is not the thread running the Subscriber
    //          and the thread is joinable, call join on the thread to wait for 
    //          it to finish.

}

bool Subscriber::ok() const {
    return impl->ok(); 
}

Subscriber::Subscriber(const std::string &topic, uint64_t node_id, const rix::msg::component::MessageInfo &msg_info,
                       std::shared_ptr<SubImplBase> impl) {
    // TODO: Implement the Susbcriber constructor. The constructor should:
    //       1. Initialize the member variables of the Subscriber.
    //       2. Initialize the fields of the ComponentInfo structure.
}

void Subscriber::add_publisher(const rix::msg::component::ID &id) {
    // TODO: Implement the add_subscriber method. This method should:
    //       1. Add the ID to the connect_pub_container in a thread-safe manner.
}

void Subscriber::remove_publisher(const rix::msg::component::ID &id) { 
    // TODO: Implement the add_subscriber method. This method should:
    //       1. Add the ID to the remove_pub_container in a thread-safe manner.
}

rix::msg::component::ID Subscriber::get_id() const {
    // TODO: Implement the get_id method. This method should:
    //       1. Return the ID of the Subscriber. The ID should contain the
    //          component ID and the URI of the Subscriber.

    return rix::msg::component::ID();
}

void Subscriber::start() {
    // TODO: Implement the start method. This method should:
    //       1. Invoke the run method of the Subscriber in a new thread.
}

void Subscriber::run() {
    thread_id = std::this_thread::get_id();
    while (impl->ok()) {
        run_once();
    }
}

void Subscriber::run_once() {
     // TODO: Implement the run_once method. This method should:
    //       1. Lock the mutex.
    //       2. If the connect_pub_container is not empty, call the connect_clients
    //          method of the implementation with the connect_pub_container.
    //       3. If the remove_pub_container is not empty, call the remove_clients
    //          method of the implementation with the remove_pub_container.
    //       4. Call the handle_message method of the implementation.
    //       5. Unlock the mutex.
}

}  // namespace core
}  // namespace rix