// ************************************************************************** //
// * This file is a simple node that publishes Timestamp messages at a      * // 
// * fixed rate. In order for the MBot to run, you must execute this        * // 
// * This file contains code that will help you understand how to use the   * //
// * rix library.                                                           * //
// ************************************************************************** //

#include <iostream>
#include <string>

#include "rix/util/cl_parser.hpp"
#include "rix/util/logger.hpp"
#include "rix/core/node.hpp"
#include "rix/core/publisher.hpp"
#include "rix/msg/mbot/Timestamp.hpp"

using Node = rix::core::Node;
using Publisher = rix::core::Publisher;
using PubImplTCP = rix::core::PubImplTCP;
using CLParser = rix::util::CLParser;

using Timestamp = rix::msg::mbot::Timestamp;

int main(int argc, char **argv) {
    CLParser parser("timesync_pub", "Timesync publisher");
    parser.add_opt(CLParser::Opt("ip", "i", "RIX Hub IP Address", rix::ipc::get_public_ip(), "", '1'));
    parser.add_opt(CLParser::Opt("freq", "f", "Publish rate in Hz", "5", "", '1'));
    parser.parse(argc, argv);

    std::string hub_ip = parser.get_opt("ip").front();
    double freq = std::stod(parser.get_opt("freq").front());

    Node& node = Node::get_instance();
    node.init("timesync_pub", hub_ip, RIX_HUB_PORT);

    std::shared_ptr<Publisher> pub = node.advertise<Timestamp, PubImplTCP>("timesync");
    if (!pub) {
        Logger::log<LogLevel::ERROR>("Failed to create publisher");
        return -1;
    }
    
    node.spin(false);

    rix::util::Rate rate(freq);

    int i = 0;
    while (node.ok()) {
        Timestamp msg;
        msg.utime = rix::util::micros();
        pub->publish(msg);
        rate.sleep();
    }   

    return 0;
}