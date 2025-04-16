#define RIX_UTIL_LOG_LEVEL 0

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "rix/core/common.hpp"
#include "rix/core/node.hpp"
#include "rix/core/subscriber.hpp"
#include "rix/msg/sensor/JS.hpp"
#include "rix/util/cl_parser.hpp"
#include "rix/util/log.hpp"

using Log = rix::util::Log;
using CLParser = rix::util::CLParser;
using JS = rix::msg::sensor::JS;
using Protocol = rix::core::Protocol;
using Node = rix::core::Node;
using rix::core::Subscriber;

void log_joint_state(const JS &msg) {
    std::stringstream ss;
    ss << "JS (t= " << msg.stamp.sec << "." << msg.stamp.nsec << ")" << std::endl;
    for (const auto &joint_state : msg.joint_states) {
        ss << "  " << joint_state.name << ": " << joint_state.position << std::endl;
    }
    Log::info << ss.str();
}

int main(int argc, char **argv) {
    CLParser parser("js_listener", "Publishes header messages");
    parser.add_opt(CLParser::Opt("ip", "i", "RIX Hub IP Address", "127.0.0.1", "", '1'));
    parser.parse(argc, argv);

    std::string hub_ip = parser.get_opt("ip")[0];

    Node::init("js_listener", hub_ip, RIX_HUB_PORT);

    Subscriber js_sub = Node::subscribe<JS>("joint_states", log_joint_state);
    if (!js_sub.ok()) {
        Log::error << "Failed to subscribe to topic joint_states" << std::endl;
        return 1;
    }

    Node::spin(true);

    return 0;
}