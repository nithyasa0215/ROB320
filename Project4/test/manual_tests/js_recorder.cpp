#define RIX_UTIL_LOG_LEVEL 0

#include <iostream>
#include <memory>
#include <fstream>
#include <string>

#include "rix/core/common.hpp"
#include "rix/core/node.hpp"
#include "rix/core/subscriber.hpp"
#include "rix/msg/serializer.hpp"
#include "rix/msg/sensor/JS.hpp"
#include "rix/util/cl_parser.hpp"
#include "rix/util/log.hpp"

using Log = rix::util::Log;
using CLParser = rix::util::CLParser;
using JS = rix::msg::sensor::JS;
using Node = rix::core::Node;
using rix::core::Subscriber;

std::ofstream file;

void record_joint_state(const JS &msg) {
    rix::ipc::Buffer buffer;
    rix::msg::Serializer<JS>::serialize(msg, buffer);
    file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
}

int main(int argc, char **argv) {
    CLParser parser("js_recorder", "Publishes header messages");
    parser.add_arg(CLParser::Arg("file_path", "File to write joint states to", '1'));
    parser.add_opt(CLParser::Opt("ip", "i", "RIX Hub IP Address", "127.0.0.1", "", '1'));
    parser.parse(argc, argv);

    std::string file_path = parser.get_arg("file_path")[0];
    std::string hub_ip = parser.get_opt("ip")[0];

    Node::init("js_recorder", hub_ip, RIX_HUB_PORT);

    file.open(file_path);
    if (!file.is_open()) {
        Log::error << "Failed to open file: " << file_path << std::endl;
        return 1;
    }

    Subscriber js_sub = Node::subscribe<JS>("joint_states", record_joint_state);
    if (!js_sub.ok()) {
        Log::error << "Failed to subscribe to topic joint_states" << std::endl;
        return 1;
    }

    Node::spin(true);

    return 0;
}