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
#include "rix/util/timing.hpp"

using Log = rix::util::Log;
using CLParser = rix::util::CLParser;
using Protocol = rix::core::Protocol;
using Node = rix::core::Node;
using rix::core::Publisher;

using JS = rix::msg::sensor::JS;

int main(int argc, char **argv) {
    CLParser parser("jsp_simulator", "Publishes JS messages on topic joint_states");
    parser.add_opt(CLParser::Opt("ip", "i", "RIX Hub IP Address", "127.0.0.1", "", '1'));
    parser.parse(argc, argv);

    std::string hub_ip = parser.get_opt("ip")[0];

    if (!Node::init("jsp_simulator", hub_ip, RIX_HUB_PORT)) {
        Log::error << "Failed to initialize node" << std::endl;
        return 1;
    }

    Publisher pub = Node::advertise<JS>("joint_states");
    if (!pub.ok()) {
        Log::error << "Failed to advertise on topic joint_states" << std::endl;
        return 1;
    }

    Node::spin(false);

    while (Node::ok() && pub.get_num_subscribers() == 0) {
        rix::util::sleep(100);
    }

    rix::util::Rate rate(1);

    double q = 0.0;
    int i = 0;
    while (Node::ok()) {
        JS msg;
        int64_t nanos = rix::util::nanos();
        msg.joint_states.resize(1);
        msg.stamp.sec = nanos / 1000000000;
        msg.stamp.nsec = nanos % 1000000000;
        msg.joint_states[0].name = "base_to_upper_arm";
        msg.joint_states[0].position = q;
        msg.joint_states[0].velocity = 0.0;
        msg.joint_states[0].effort = 0.0;

        pub.publish(msg);

        q += 0.1;

        rate.sleep();
        i++;
        Log::info << "Published " << i << " messages" << std::endl;
        if (i == 10) {
            break;
        }
    }

    return 0;
}