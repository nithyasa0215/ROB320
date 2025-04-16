#include <functional>
#include <iostream>
#include <mutex>
#include <string>

#include "mbot.hpp"

#include "rix/util/cl_parser.hpp"
#include "rix/util/logger.hpp"
#include "rix/core/node.hpp"
#include "rix/core/publisher.hpp"
#include "rix/core/subscriber.hpp"
#include "rix/msg/mbot/Pose2D.hpp"
#include "rix/msg/mbot/Twist2D.hpp"
#include "rix/msg/mbot/Encoders.hpp"
#include "rix/msg/mbot/IMU.hpp"
#include "rix/msg/mbot/MotorPWM.hpp"
#include "rix/msg/mbot/MotorVelocity.hpp"
#include "rix/msg/mbot/Timestamp.hpp"

using Node = rix::core::Node;
using Publisher = rix::core::Publisher;
using Subscriber = rix::core::Subscriber;
using PubImplTCP = rix::core::PubImplTCP;
using SubImplTCP = rix::core::SubImplTCP;
using CLParser = rix::util::CLParser;

using Pose2D = rix::msg::mbot::Pose2D;
using Twist2D = rix::msg::mbot::Twist2D;
using Encoders = rix::msg::mbot::Encoders;
using IMU = rix::msg::mbot::IMU;
using MotorPWM = rix::msg::mbot::MotorPWM;
using MotorVelocity = rix::msg::mbot::MotorVelocity;
using Timestamp = rix::msg::mbot::Timestamp;

class MBotDriver {
    public:
        MBotDriver(const std::string &hub_ip, uint16_t hub_port) 
        : mbot("/dev/mbot_lcm") {
            // TODO: Implement the MBotDriver constructor. This constructor
            //       should:
            //       1. Initialize the Node with name "mbot_driver", the Hub IP
            //          address, and the Hub port.
            //       2. Create publishers for the MBot topics.
            //       3. Set the callback functions for the MBot topics. 
            //          You can use lambda functions (easier) or declare
            //          functions in this class. If you declare functions, use 
            //          std::bind to bind the member functions to 'this'. These
            //          callback functions will be invoked in the background 
            //          when the driver receives a message over serial from the 
            //          MBot. Check the MBot class for the topic IDs and 
            //          corresponding message types.
            //          Lambda function example:
            //              mbot.set_callback(TOPIC_ID, [this](MBot &mbot, const void *msg, size_t msg_len) {
            //                  ...
            //              });
            //          Member function example:  
            //              using std::placeholders::_1;
            //              using std::placeholders::_2; 
            //              using std::placeholders::_3;
            //              mbot.set_callback(TOPIC, std::bind(&MBotDriver::cb_func, this, _1, _2, _3));
            //       4. Create subscribers for the MBot topics. You must pass
            //          another callback to the subscribe method that will be 
            //          invoked when a message is received from the RIX Node.
            //          The subscriber callback should send the message to the 
            //          MBot. As above, you can use lambda functions or declare 
            //          functions.
            //          Lambda function example:
            //              sub = node.subscribe<TMsg>("topic", [this](const TMsg &msg) {
            //                  ...
            //              });
            //          Member function example:
            //              sub = node.subscribe<TMsg>("topic", std::bind(&MBotDriver::sub_cb, this, _1));
        }

        ~MBotDriver() {
            // TODO: Implement the MBotDriver destructor. This destructor 
            //       should:
            //       1. Stop the MBot.
        }
    
        void run() {
            // TODO: Implement the run method. This method should:
            //       1. Start the MBot in non-blocking mode.
            //       2. Spin the RIX Node in blocking mode.
        }
    
    private:
        MBot mbot;
        Node &node = Node::get_instance();

        // TODO: Declare publishers and subscribers for the MBot topics. Use
        //       shared pointers to the Publisher and Subscriber classes.

        // TODO: Declare callback functions for the MBot topics (If you don't 
        //       want to use lambda functions).
};

int main(int argc, char **argv) {
    CLParser parser("mbot_driver", "RIX driver for the Robot Control Board");
    parser.add_opt(CLParser::Opt("ip", "i", "RIX Hub IP Address", rix::ipc::get_public_ip(), "", '1'));
    parser.parse(argc, argv);

    // TODO: Implement the main function. This program should:
    //       1. Parse the command line arguments to get the RIX Hub IP address.
    //       2. Create an MBotDriver object with the RIX Hub IP address and the
    //          RIX Hub port.
    //       3. Run the MBotDriver object.

    return 0;
}
