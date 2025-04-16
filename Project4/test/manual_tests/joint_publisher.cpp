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
using Protocol = rix::core::Protocol;
using Node = rix::core::Node;
using rix::core::Publisher;

using JS = rix::msg::sensor::JS;

int main(int argc, char **argv) {
    CLParser parser("joint_publisher", "Publishes header messages");
    parser.add_arg(CLParser::Arg("topic", "Topic name", '1'));
    parser.add_opt(CLParser::Opt("ip", "i", "RIX Hub IP Address", "127.0.0.1", "", '1'));
    parser.add_opt(CLParser::Opt("rate", "r", "Publish rate in Hz", "25", "", '1'));
    parser.add_opt(CLParser::Opt("lower_bound", "l", "Lower bound of joint position", "-3.1415", "", '1'));
    parser.add_opt(CLParser::Opt("upper_bound", "u", "Upper bound of joint position", "3.1415", "", '1'));
    parser.add_opt(CLParser::Opt("increment", "n", "Increment of joint position", "0.1", "", '1'));
    parser.parse(argc, argv);

    std::string topic = parser.get_arg("topic")[0];
    std::string hub_ip = parser.get_opt("ip")[0];
    int hz = std::stoi(parser.get_opt("rate")[0]);
    double lower_bound = std::stod(parser.get_opt("lower_bound")[0]);
    double upper_bound = std::stod(parser.get_opt("upper_bound")[0]);
    double increment = std::stod(parser.get_opt("increment")[0]);

    if (!Node::init("joint_publisher", hub_ip, RIX_HUB_PORT)) {
        Log::error << "Failed to initialize node" << std::endl;
        return 1;
    }

    Publisher pub = Node::advertise<JS>(topic);
    if (!pub.ok()) {
        Log::error << "Failed to advertise on topic " << topic << std::endl;
        return 1;
    }

    Node::spin(false);
    while (Node::ok() && pub.get_num_subscribers() == 0) {
        rix::util::sleep(100);
    }

    rix::util::Rate rate(hz);

    int i = 0;
    double q = 0.0;
    while (Node::ok()) {
        JS msg;
        int64_t nanos = rix::util::nanos();
        msg.joint_states.resize(1);
        msg.stamp.sec = nanos / 1000000000;
        msg.stamp.nsec = nanos % 1000000000;
        msg.joint_states[0].name = topic;
        msg.joint_states[0].position = q;
        msg.joint_states[0].velocity = 0.0;
        msg.joint_states[0].effort = 0.0;

        pub.publish(msg);

        q += increment;
        if (q + increment > upper_bound || q + increment < lower_bound) {
            increment = -increment;
        }

        i++;
        Log::info << "Published " << i << " messages" << std::endl;
        if (i == 10) {
            break;
        }

        rate.sleep();
    }

    return 0;
}