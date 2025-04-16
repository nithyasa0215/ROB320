#define RIX_UTIL_LOG_LEVEL 0

#include <functional>
#include <iostream>
#include <mutex>
#include <nlohmann/json.hpp>
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

#include "rix/msg/geometry/TF.hpp"
#include "rix/msg/sensor/JS.hpp"

#include "rix/rdf/tree.hpp"



#include <fstream>	

using Log = rix::util::Log;
using CLParser = rix::util::CLParser;

/**
 * TODO: Declare any classes or helper functions you need here.
 */

 // here is where you will declare are create the RSP
 class RSP{
    public:
    RSP(const std::string &hub_ip, uint16_t hub_port, const std::string &jrdf, double rate)
        : tree(rix::rdf::Json::parse(jrdf)), max_publish_period(1.0 / rate) {

    rix::core::Node::init("jointState", hub_ip, hub_port);
    subscriber = rix::core::Node::subscribe<rix::msg::sensor::JS>("jointState",[this](const rix::msg::sensor::JS &msg) {
        this->callback(msg);
    });

    publisher = rix::core::Node::advertise<rix::msg::geometry::TF>("tf");
    }
    
    void run(){
        rix::core::Node::spin(true);
    }

    private:
        rix::core::Publisher publisher; 
        rix::core::Subscriber subscriber;
        rix::rdf::Tree tree;
        std::mutex mutex; 

        double max_publish_period;
        int64_t last_publish_time = 0;

        void callback(const rix::msg::sensor::JS &msg) {
            int current = static_cast<int64_t>(msg.stamp.sec) * 1e9 + msg.stamp.nsec;
            if (current - last_publish_time >= max_publish_period * 1e9) {

                std::lock_guard<std::mutex> lock(mutex);
                tree.set_state(msg);
                rix::msg::geometry::TF tf_msg = tree.TF();
                publisher.publish(tf_msg);
                last_publish_time = current;
            }
        }


 };

int main(int argc, char **argv)
{
    CLParser parser("tf_publisher", "Publishes header messages");
    parser.add_arg(CLParser::Arg("jrdf", "Path to JRDF file", '1'));
    parser.add_opt(CLParser::Opt("ip", "i", "RIX Hub IP Address", "127.0.0.1", "", '1'));
    parser.parse(argc, argv);
    parser.add_opt(CLParser::Opt("rate", "r", "Publishing rate (Hz)", "10", "", '1'));

 
    std::string jrdf_file = parser.get_arg("jrdf").front();
    std::string hub_ip = parser.get_opt("ip").front();
    double rate = std::stod(parser.get_opt("rate").front());

    /**
     * TODO: Implement the Robot State Publisher
     */

     std::ifstream file(jrdf_file);
     std::string name((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
     RSP rsp(hub_ip,RIX_HUB_PORT, name, rate);

     rsp.run();
    //return 0;

    return 0;
}