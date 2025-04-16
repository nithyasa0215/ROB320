#include "rix/core/subscriber/sub_impl_tcp.hpp"

namespace rix {
namespace core {

SubImplTCP::SubImplTCP(size_t msg_size, uint64_t component_id, SubCallbackPtr callback)
    : SubImplBase(msg_size, component_id, callback) {
    // TODO: Implement the Subscriber constructor. The constructor should:
    //       1. Initialize the member variables of the Subscriber class.
}

SubImplTCP::~SubImplTCP() {
    // TODO: Implement the destructor. The destructor should:
    //       1. Do nothing by default, unless you need to perform cleanup for
    //          any member variables that you have created.
}

void SubImplTCP::connect_clients(std::queue<rix::msg::component::ID> &pubs) {
    // TODO: Implement the connect_clients method. This method should:
    //       1. While the Subscriber is running and there are publishers in the
    //          container:
    //          a. Create a Client shared pointer with the URI of the publisher
    //             that is at the front of the pubs queue.
    //          b. Connect to the publisher server.
    //          c. Send the subscriber's rix::msg::component::ID to the 
    //             publisher after a successful connection.
    //          d. Insert the client into the clients map.
    //       2. If an error occurs at any step, continue to the next publisher.
}

void SubImplTCP::remove_clients(std::queue<rix::msg::component::ID> &pubs) {
    // TODO: Implement the remove_clients method. This method should:
    //       1. Remove the clients from the clients map that match the component
    //          IDs in the pubs queue. If a client is not found, continue to the
    //          next client.

}

void SubImplTCP::handle_message() {
    // TODO: Implement the handle_msg method. This method should:
    //       1. Receive a message from all TCP clients that are readable. If an
    //          error occurs, continue to the next client.
    //       2. If a message is received successfully, call the callback with the
    //          message. Check the function signature of the callback in the
    //          SubscriberCallbackBase class.

}

uint8_t SubImplTCP::get_protocol() { 
    return Protocol::TCP; 
}

size_t SubImplTCP::get_num_publishers() { 
    // TODO: Implement the get_num_publishers method. This method should:
    //       1. Return the number of clients in the clients map.

    return 0;
}

}  // namespace core
}  // namespace rix