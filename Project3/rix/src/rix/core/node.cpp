#include "rix/core/node.hpp"

namespace rix {
namespace core {

Node &Node::get_instance() {
    static Node instance;
    return instance;
}

Node::~Node() {
    // TODO: Implement the destructor. This method should:
    //       1. Call the shutdown method.
}

bool Node::init(const std::string &name, const std::string &hub_ip, uint16_t hub_port) {
    if (initialized) {
        Logger::log<LogLevel::WARN>("Node has already been initialized");
        return false;
    }

    // TODO: Implement the init method. This method should:
    //       1. Initialize the logger with the given name and disable logging 
    //          to file.
    //       2. Register signal handlers for SIGINT, SIGTERM, and SIGPIPE.
    //       3. Initialize the name member variable with the given name 
    //          parameter.
    //       4. Initialize the hub_uri member variable with the given hub IP
    //          and port.
    //       5. Initialize the client member variable with a shared pointer to
    //          a new Client instance. The client should be initialized with
    //          the hub URI.
    //       6. Generate a random 64-bit node ID.
    //       7. Connect to the hub. 
    //          a. Attempt to connect to the hub every 100 ms for a maximum of 
    //             10 attempts.
    //          b. If the connection fails or times out, return false. 
    //       8. Set the initialized flag to true if the connection is 
    //          successful and return true.

    return false;
}

void Node::spin(bool block) {
    Logger::log<LogLevel::DEBUG>("Node::spin called.");
    if (!initialized) {
        Logger::log<LogLevel::ERROR>("Node has not been initialized");
        return;
    }
    if (spinning) {
        Logger::log<LogLevel::WARN>("Node is already spinning");
        return;
    }

    // TODO: Implement the spin method. This method should:
    //       1. Invoke the run method in a new thread.
    //       2. Set the spinning flag to true.
    //       3. If the block parameter is true, join the thread.
}

void Node::spin_once() {
    if (!initialized) {
        Logger::log<LogLevel::ERROR>("Node has not been initialized");
    }
    if (spinning) {
        Logger::log<LogLevel::WARN>("Node: already spinning");
        return;
    }
    run_once();
}

void Node::shutdown() {
    Logger::log<LogLevel::DEBUG>("Node::shutdown called.");
    if (!initialized) {
        return;
    }

    // TODO: Implement the shutdown method. This method should:
    //       1. Set the shutdown flag to true.
    //       2. Join the spin thread if it is running on a different thread and
    //          is joinable. (check that the spin_thread_id is not the same as
    //          the current thread id)
    //       3. Deregister all publishers and subscribers.
    //       4. Clear the publishers and subscribers containers.
    //       5. Close the client connection.
}

bool Node::ok() {
    if (!initialized) {
        Logger::log<LogLevel::ERROR>("Node has not been initialized");
    }
    return !shutdown_flag;
}

Node::Node() {
    // TODO: Implement the Node constructor. This method should:
    //       1. Initialize the flags to false.
    //       2. Initialize the machine ID to 0.
    //       3. Initialize the node ID to 0.
}

void Node::signal_handler(int signal) {
    // TODO: Implement the signal_handler method. This method should:
    //       1. Set the shutdown flag of the Node instance to true if the signal
    //          is SIGINT or SIGTERM.
}

void Node::run() {
    spin_thread_id = std::this_thread::get_id();
    while (ok()) {
        try {
            run_once();
        } catch (const std::exception &e) {
            Logger::log<LogLevel::ERROR>("Error thrown in Node::run(): " + std::string(e.what()));
            return;
        }
    }
}

void Node::run_once() {
    // TODO: Implement the run_once method. This method should:
    //       1. Check if the client is readable in a thread-safe manner.
    //       2. If the client is readable, receive a message from the hub in
    //          a thread-safe manner.
    //       3. Decode the message and handle it based on the opcode.
    //       4. Iterate over all publishers and subscribers and check if they
    //          have requested shutdown. If they have, deregister it, call the
    //          implementation's shutdown method, and remove it from its
    //          respective container.
}

bool Node::register_publisher(std::shared_ptr<Publisher> pub) {
    // TODO: Implement the register_publisher method. This method should:
    //       1. Create an Info message with the PUB_REGISTER opcode and the
    //          publisher's ComponentInfo and ID.
    //       2. Encode the message and send it to the hub (use the client_mutex
    //          to ensure thread safety).
    //       3. If an error occurs, return false.

    return false;
}

bool Node::register_subscriber(std::shared_ptr<Subscriber> sub) {
    // TODO: Implement the register_subscriber method. This method should:
    //       1. Create an Info message with the SUB_REGISTER opcode and the
    //          subscriber's ComponentInfo and ID.
    //       2. Encode the message and send it to the hub (use the client_mutex
    //          to ensure thread safety).
    //       3. If an error occurs, return false.

    return false;
}

bool Node::deregister_publisher(std::shared_ptr<Publisher> pub) {
    // TODO: Implement the deregister_publisher method. This method should:
    //       1. Create an Info message with the PUB_DEREGISTER opcode and the
    //          publisher's ComponentInfo.
    //       2. Encode the message and send it to the hub (use the client_mutex
    //          to ensure thread safety).
    //       3. If an error occurs, return false.

    return false;
}

bool Node::deregister_subscriber(std::shared_ptr<Subscriber> sub) {
    // TODO: Implement the deregister_subscriber method. This method should:
    //       1. Create an Info message with the SUB_DEREGISTER opcode and the
    //          subscriber's ComponentInfo.
    //       2. Encode the message and send it to the hub (use the client_mutex
    //          to ensure thread safety).
    //       3. If an error occurs, return false.

    return false;
}

void Node::handle_pub_notify(const rix::msg::component::Info *notify) {
    // TODO: Implement the handle_pub_notify method. This method should:
    //       1. If the notify message has an error, return.
    //       2. Otherwise, add the contact_id of the subscriber to the 
    //          accept_sub_container of the publisher that matches the
    //          component info in the notify message.
}

void Node::handle_sub_notify(const rix::msg::component::Info *notify) {
    // TODO: Implement the handle_sub_notify method. This method should:
    //       1. If the notify message has an error return.
    //       2. Otherwise, add the contact_id of the publisher to the
    //          connect_pub_container of the subscriber that matches the
    //          component info in the notify message.
}

void Node::handle_pub_disconnect(const rix::msg::component::Info *disconnect) {
    // TODO: Implement the handle_pub_disconnect method. This method should:
    //       1. Add the contact_id of the subscriber to the 
    //          remove_sub_container of the publisher that matches the
    //          component info in the disconnect message.
}

void Node::handle_sub_disconnect(const rix::msg::component::Info *disconnect) {
    // TOOD: Implement the handle_sub_disconnect method. This method should:
    //       1. Add the contact_id of the publisher to the
    //          remove_pub_container of the subscriber that matches the
    //          component info in the disconnect message.
}

}  // namespace core
}  // namespace rix
