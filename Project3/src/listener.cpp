#include <iostream>
#include <string>

#include "rix/util/cl_parser.hpp"
#include "rix/util/logger.hpp"
#include "rix/core/node.hpp"
#include "rix/core/subscriber.hpp"
#include "rix/msg/standard/Header.hpp"

using Node = rix::core::Node;
using Subscriber = rix::core::Subscriber;
using SubImplTCP = rix::core::SubImplTCP;
using CLParser = rix::util::CLParser;

using Header = rix::msg::standard::Header;

void callback(const Header &msg) {
    Logger::log<LogLevel::INFO>("seq: %u, stamp: %u.%u, frame_id: %s", msg.seq, msg.stamp.sec, msg.stamp.nsec, msg.frame_id);
}

int main(int argc, char **argv) {
    CLParser parser("listener", "Simple talker program");
    parser.add_opt(CLParser::Opt("ip", "i", "RIX Hub IP Address", rix::ipc::get_public_ip(), "", '1'));
    parser.parse(argc, argv);
    std::string hub_ip = parser.get_opt("ip").front();
    
    // TODO: Implement the listener program. This program should:
    //       1. Get a reference to the Node class and initialize it with name
    //          "listener", the Hub IP address, and the Hub port.
    //       2. Create a subscriber on the topic "topic" with message type 
    //          Header. Use the callback function provided for you. If the node
    //          fails to create the Subscriber, return 1.
    //       3. Spin the node in blocking mode.
    //       4. Return 0 once the node has finished spinning.

    return 0;
}