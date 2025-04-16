// ************************************************************************** //
// * This program is a simple listener program that will log a message when * //
// * a LidarScan message is received over the "lidar_scan" topic. This      * //
// * program is provided for you so you can see how to use the RIX library  * //
// * is used from the user end.                                             * //
// ************************************************************************** //

#include <iostream>
#include <string>

#include "rix/util/cl_parser.hpp"
#include "rix/util/logger.hpp"
#include "rix/core/node.hpp"
#include "rix/core/subscriber.hpp"
#include "rix/msg/mbot/LidarScan.hpp"

using Node = rix::core::Node;
using Subscriber = rix::core::Subscriber;
using SubImplTCP = rix::core::SubImplTCP;
using CLParser = rix::util::CLParser;

using LidarScan = rix::msg::mbot::LidarScan;

void callback(const LidarScan &msg) {
    Logger::log<LogLevel::INFO>("Received scan: utime: %lld, num_ranges: %u", msg.utime, msg.num_ranges);
}

int main(int argc, char **argv) {
    CLParser parser("lidar_listener", "Logs messages on the \"lidar_scan\" topic");
    parser.add_opt(CLParser::Opt("ip", "i", "RIX Hub IP Address", rix::ipc::get_public_ip(), "", '1'));
    parser.parse(argc, argv);

    std::string hub_ip = parser.get_opt("ip").front();

    Node& node = Node::get_instance();
    if (!node.init("lidar_listener", hub_ip, RIX_HUB_PORT)) {
        Logger::log<LogLevel::FATAL>("Failed to initialize node");
        return 1;
    }

    std::shared_ptr<Subscriber> sub = node.subscribe<LidarScan>("lidar_scan", callback);
    if (!sub) {
        Logger::log<LogLevel::ERROR>("Failed to create subscriber");
        return 1;
    }
    
    node.spin();
    return 0;
}