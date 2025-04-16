#include "rix/core/publisher/pub_impl_tcp.hpp"

namespace rix {
namespace core {

PubImplTCP::PubImplTCP(size_t msg_size, uint64_t component_id) : PubImplBase(msg_size, component_id) {
    // TODO: Implement the constructor. The constructor should:
    //       1. Initialize the TCP server with the public IP address and a port
    //          of 0.
    //       2. Bind the server.
    //       3. Listen on the server with a maximum backlog of INT16_MAX.
}

PubImplTCP::~PubImplTCP() {
    // TODO: Implement the destructor. The destructor should:
    //       1. Do nothing by default, unless you need to perform cleanup for
    //          any member variables that you have created.
}

rix::msg::component::URI PubImplTCP::get_uri() const {
    // TODO: Implement the get_uri method. This method should:
    //       1. Get the URI of the server.
    //       2. Create a rix::msg::component::URI message and set the address
    //          and port of the URI.
    //       3. Return the URI message.

    return rix::msg::component::URI();
}

void PubImplTCP::transport_message(const uint8_t *msg, size_t len) {
    // TODO: Implement the transport_message method. This method should:
    //       1. Send the message to all writable connections in the conns map.
    //          If an error occurs, continue to the next connection.
}

void PubImplTCP::accept_connections(std::map<uint64_t, rix::msg::component::ID> &subs) {
    // TODO: Implement the accept_connections method. This method should:
    //       1. While the Publisher is running and there are subscribers in the
    //          container:
    //          a. Accept a TCP connection.
    //          b. If successful, receive the component ID from the subscriber.
    //          c. If the component ID is not in the conns map, add the connection
    //             to conns. Otherwise, continue to the next subscriber in the 
    //             subs map. 
    //          d. Remove the subscriber from the subs map.
}

void PubImplTCP::remove_connections(std::queue<rix::msg::component::ID> &subs) {
    // TODO: Implement the remove_connections method. This method should:
    //       1. Remove the connections from the conns map that match the 
    //          component IDs in the subs queue. If the component ID is not in
    //          the conns map, ignore it.
}

uint8_t PubImplTCP::get_protocol() {
    return Protocol::TCP; 
}

size_t PubImplTCP::get_num_subscribers() { 
    // TODO: Implement the get_num_subscribers method. This method should:
    //       1. Return the number of connections in the conns map.
    
    return 0;
}

}  // namespace core
}  // namespace rix