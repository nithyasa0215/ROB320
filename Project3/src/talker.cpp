#include <iostream>
#include <string>

#include "rix/util/cl_parser.hpp"
#include "rix/util/logger.hpp"
#include "rix/core/node.hpp"
#include "rix/core/publisher.hpp"
#include "rix/msg/standard/Header.hpp"

using Node = rix::core::Node;
using Publisher = rix::core::Publisher;
using PubImplTCP = rix::core::PubImplTCP;
using CLParser = rix::util::CLParser;

using Header = rix::msg::standard::Header;

int main(int argc, char **argv) {
    CLParser parser("talker", "Simple talker program");
    parser.add_opt(CLParser::Opt("ip", "i", "RIX Hub IP Address", rix::ipc::get_public_ip(), "", '1'));
    parser.add_opt(CLParser::Opt("freq", "f", "Publish rate in Hz", "5", "", '1'));
    parser.parse(argc, argv);

    std::string hub_ip = parser.get_opt("ip").front();
    double freq = std::stod(parser.get_opt("freq").front());

    // TODO: Implement the talker program. This program should:
    //       1. Get a reference to the Node class and initialize it with name
    //          "talker", the Hub IP address, and the Hub port.
    //       2. Create a publisher on topic "topic" with message type Header.
    //          If the node fails to create the Publisher, return 1.
    //       3. Spin the node in the background
    //       4. Publish Header messages with frame_id "talker". The Header::seq
    //          field should be incremented in every message. The Header::stamp
    //          field should include the current epoch time. The loop should run
    //          at the frequency specified by the command line arguments. Use 
    //          the rix::util::Rate class to control the loop frequency. Use the
    //          Node::ok method to break out of the loop once the node has
    //          shutdown.

    return 0;
}