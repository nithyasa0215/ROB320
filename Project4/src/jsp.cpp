#define RIX_UTIL_LOG_LEVEL 1

#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "rix/util/cl_parser.hpp"
#include "rix/util/log.hpp"
#include "rix/util/timing.hpp"

#include "rix/core/common.hpp"
#include "rix/core/node.hpp"
#include "rix/core/publisher.hpp"
#include "rix/core/subscriber.hpp"

#include "rix/msg/sensor/JS.hpp"
#include "rix/msg/sensor/JointState.hpp"

#include "rix/rdf/tree.hpp"

using CLParser = rix::util::CLParser;
using Log = rix::util::Log;

/**
 * TODO: Declare any classes or helper functions you need here.
 */

Class JSP{
    public:
    JSP(const std::string &ip, uint16_t port, const std::string &jrdf, double rate)
    : tree(rix::rdf::Json::parse(jrdf)), publish_period(1.0 / rate) {
    
    rix::core::Node::init("joint_state_publisher", ip, port);

};

int main(int argc, char **argv) {
    CLParser parser("joint_state_publisher", "Publishes header messages");
    parser.add_arg(CLParser::Arg("jrdf",   "Path to JRDF file",       '1'));
    parser.add_arg(CLParser::Arg("topics", "Topics to subscribe to",  '+'));
    parser.add_opt(CLParser::Opt("ip",   "i", "RIX Hub IP Address", "127.0.0.1", "", '1'));
    parser.add_opt(CLParser::Opt("rate", "r", "Publish rate in Hz", "50",        "", '1'));
    parser.parse(argc, argv);

    std::string jrdf_file = parser.get_arg("jrdf").front();
    std::vector<std::string> topics = parser.get_arg("topics");
    std::string hub_ip = parser.get_opt("ip").front();
    int rate = std::stoi(parser.get_opt("rate").front());

    /**
     * TODO: Implement the Joint State Publisher
     */

     // similar idea to rsp 

    std::ifstream file(jrdf_file);
    std::string jrdf((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    JSP jsp(hub_ip, RIX_HUB_PORT, jrdf, rate);
    jsp.run();
    return 0;
}