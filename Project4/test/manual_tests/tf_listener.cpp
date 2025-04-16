#define RIX_UTIL_LOG_LEVEL 0

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "rix/core/common.hpp"
#include "rix/core/node.hpp"
#include "rix/core/subscriber.hpp"
#include "rix/msg/geometry/TF.hpp"
#include "rix/rdf/fk_solver.hpp"
#include "rix/rdf/tree.hpp"
#include "rix/util/cl_parser.hpp"
#include "rix/util/log.hpp"

using Log = rix::util::Log;
using CLParser = rix::util::CLParser;
using Node = rix::core::Node;
using Subscriber = rix::core::Subscriber;
using TF = rix::msg::geometry::TF;

void log_tf(const TF &msg) {
    std::stringstream ss;
    ss << "TF:\n";
    for (const auto &t : msg.transforms) {
        ss << t.header.frame_id << " -> " << t.child_frame_id << "\n";
        ss << "Translation: [x: " << t.transform.translation.x << ", y: " << t.transform.translation.y
           << ", z: " << t.transform.translation.z << "]\n";
        ss << "Rotation: [w: " << t.transform.rotation.w << ", x: " << t.transform.rotation.x
           << ", y: " << t.transform.rotation.y << ", z: " << t.transform.rotation.z << "]\n";
    }
    Log::info << ss.str() << std::endl;
}

int main(int argc, char **argv) {
    CLParser parser("tf_listener", "Logs TF messages");
    parser.add_opt(CLParser::Opt("ip", "i", "RIX Hub IP Address", "127.0.0.1", "", '1'));
    parser.parse(argc, argv);

    std::string hub_ip = parser.get_opt("ip")[0];

    Node::init("tf_listener", hub_ip, RIX_HUB_PORT);
    Subscriber tf_sub = Node::subscribe<TF>("tf", log_tf);
    if (!tf_sub.ok()) {
        Log::error << "Failed to subscribe to topic tf" << std::endl;
        return 1;
    }
    Node::spin(true);
    return 0;
}