#include "mediator.hpp"

namespace rix {
namespace core {

Mediator::~Mediator() {
    Logger::log<LogLevel::DEBUG>("Mediator::~Mediator called.");
    shutdown();
    Logger::log<LogLevel::DEBUG>("Mediator::~Mediator returning.");
}

Mediator &Mediator::get_instance() {
    static Mediator instance;
    return instance;
}

Mediator::Mediator() {
    // TODO: Implement the Mediator constructor. This method should:
    //       1. Set the flags to false.
    //       2. Initialize uri with the public IP and RIX_HUB_PORT.
    //       3. Initialize the server with uri.
}

void Mediator::init() {
    // TODO: Implement the init method. This method should:
    //       1. Register sigint_handler for SIGINT, SIGTERM, and SIGPIPE.
    //       2. Initialize the logger with the name "rixhub" and disable 
    //          logging to file.
    //       3. Bind the server.
    //       4. Listen on the server with a maximum backlog of INT16_MAX.
}

void Mediator::shutdown() {
    Logger::log<LogLevel::DEBUG>("Mediator::shutdown called.");
    // TODO: Implement the shutdown method. This method should:
    //       1. Set the shutdown_flag to true.
    //       2. Send an Info message to all nodes with the opcode MED_TERMINATE.
    //       3. Clear the nodes map.
}

bool Mediator::ok() { 
    // TODO: Implement the ok method. This method should:
    //       1. Return true if the shutdown_flag is false.

    return false;
}

void Mediator::spin(bool block) {
    // TODO: Implement the spin method. This method should:
    //       1. Invoke the run method in a new thread.
    //       2. If block is true, join the thread.

}

void Mediator::sigint_handler(int signal) { 
    // TODO: Implement the sigint_handler method. This method should:
    //       1. Set the shutdown_flag of the Mediator instance to true if the
    //          signal is SIGINT or SIGTERM. Note that this method is static but
    //          still has access to the private members of the Mediator class.
}

void Mediator::run() {
    // TODO: Implement the run method. This method should:
    //       1. Set the spinning flag to true.
    //       2. While the ok method returns true:
    //          a. Accept a new TCP connection.
    //          b. If accept was successful, add the connection to the conns 
    //             vector.
    //       3. Receive messages from all TCP connections that are readable.
    //          a. If the message was received successfully, decode and handle 
    //             it.
    //          b. Check if we have already received a message from this 
    //             connection using the node_id. If not, add it to the nodes 
    //             map.
    //          c. Handle the message.
}

void Mediator::handle_msg(const Info &in_msg) {
    // TODO: Implement the handle_msg method. This method should:
    //       1. Check the opcode of the message.
    //       2. Call the appropriate method based on the opcode.
}

void Mediator::register_publisher(const ComponentInfo &component_info, const ID &id) {
    // TODO: Implement the register_publisher method. This method should:
    //       1. Validate the topic string (ensure it is not empty).
    //       2. Add the topic to the ledger.
    //          a. If the topic does not exist, add it to the ledger.
    //          b. If the topic exists, check if the message type is the same.
    //          c. If the message type is different, set the error code to
    //             WRONG_MSG_TYPE.
    //       3. If the error code was not previously set, add the publisher to
    //          the ledger. If the component already exists in the ledger, set
    //          the error code to PUB_EXISTS.
    //       4. If there was an error, send PUB_NOTIFY with the error code and
    //          return.
    //       5. Send notifications to the publisher and all subscribers on the
    //          topic.
    //          a. Get all subscribers for the topic.
    //          b. Initialize the SUB_NOTIFY and PUB_NOTIFY messages.
    //             Each SUB_NOTIFY message should contain the publisher's
    //             contact ID and the subscriber's component info.
    //             Each PUB_NOTIFY message should contain the publisher's 
    //             component info and the subscriber's contact ID.
    //          c. Send PUB_NOTIFY to the publisher for each subscriber.
    //          d. Send SUB_NOTIFY to all subscribers.
}

void Mediator::register_subscriber(const ComponentInfo &component_info, const ID &id) {
    // TODO: Implement the register_subscriber method. This method should:
    //       1. Validate the topic string (ensure it is not empty).
    //       2. Add the topic to the ledger.
    //          a. If the topic does not exist, add it to the ledger.
    //          b. If the topic exists, check if the message type is the same.
    //          c. If the message type is different, set the error code to
    //             WRONG_MSG_TYPE.
    //       3. If the error code was not previously set, add the subscriber to
    //          the ledger. If the component already exists in the ledger, set
    //          the error code to SUB_EXISTS.
    //       4. If there was an error, send SUB_NOTIFY with the error code and
    //          return.
    //       5. Send notifications to the subscriber and all publishers on the
    //          topic.
    //          a. Get all publishers for the topic.
    //          b. Initialize the SUB_NOTIFY and PUB_NOTIFY messages.
    //             Each SUB_NOTIFY message should contain the subscriber's
    //             contact ID and the publisher's component info.
    //             Each PUB_NOTIFY message should contain the publisher's
    //             component info and the subscriber's contact ID.
    //          c. Send SUB_NOTIFY to the subscriber for each publisher.
    //          d. Send PUB_NOTIFY to all publishers.
}

void Mediator::deregister_publisher(const ComponentInfo &component_info) {
    // TODO: Implement the deregister_publisher method. This method should:
    //       1. Check if the component exists in the ledger. If it does not,
    //          return.
    //       2. Remove the publisher from the ledger.
    //       3. Send an Info message with the PUB_DISCONNECT opcode to all
    //          subscribers of the publisher's topic. This message should 
    //          contain the publisher's contact ID and the subscriber's 
    //          component info.
}

void Mediator::deregister_subscriber(const ComponentInfo &component_info) {
    // TODO: Implement the deregister_subscriber method. This method should:
    //       1. Check if the component exists in the ledger. If it does not,
    //          return.
    //       2. Remove the subscriber from the ledger.
    //       3. Send an Info message with the PUB_DISCONNECT opcode to all
    //          publishers of the subscriber's topic. This message should
    //          contain the subscriber's contact ID and the publisher's
    //          component info.
}

void Mediator::send(uint64_t node_id, const Info &msg) {
    // This is a helper function to send an Info message to a node. If the node
    // is not found, the function will log an error message and remove the node
    // from the nodes map.
    const uint8_t *out_msg = msg.encode();
    size_t send_size = sizeof(Info);
    if (nodes.find(node_id) != nodes.end()) {
        int status = nodes[node_id]->send(out_msg, send_size);
        if (status < 0) {
            Logger::log<LogLevel::DEBUG>("Failed to send message.");
            nodes.erase(node_id);
        }
    } else {
        Logger::log<LogLevel::DEBUG>("Node %llu not found", node_id);
    }
}

}  // namespace core
}  // namespace rix