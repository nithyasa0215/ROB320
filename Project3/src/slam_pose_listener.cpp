// ************************************************************************** //
// * This program is a simple listener program that will log a message when * //
// * a Pose2D message is received over the "slam_pose" topic. This program  * //
// * is provided for you so you can see how to use the RIX library is used  * //
// * from the user end.                                                     * //
// ************************************************************************** //

#include <iostream>
#include <memory>
#include <string>

#include "rix/core/node.hpp"
#include "rix/core/publisher.hpp"
#include "rix/msg/mbot/Pose2D.hpp"
#include "rix/util/timing.hpp"
#include "rix/util/cl_parser.hpp"
#include "rix/util/logger.hpp"

using CLParser = rix::util::CLParser;
using Node = rix::core::Node;
using Subscriber = rix::core::Subscriber;
using Pose2D = rix::msg::mbot::Pose2D;

void callback(const Pose2D &msg) {
    Logger::log<LogLevel::INFO>("x: %f, y: %f, theta: %f", msg.x, msg.y, msg.theta);
}

int main(int argc, char **argv) {
    CLParser parser("slam_pose_sub", "Logs slam pose");
    parser.add_opt(CLParser::Opt("ip", "i", "RIX Hub IP Address", rix::ipc::get_public_ip(), "", '1'));
    parser.parse(argc, argv);

    std::string hub_ip = parser.get_opt("ip").front();

    Node &node = Node::get_instance();
    node.init("slam_pose_sub", hub_ip, RIX_HUB_PORT);

    std::shared_ptr<Subscriber> sub = node.subscribe<Pose2D>("slam_pose", callback);
    if (!sub) {
        Logger::log<LogLevel::ERROR>("Failed to create subscriber");
        return -1;
    }

    node.spin();
}
