#include <functional>
#include <iostream>
#include <mutex>
#include <string>

#include "rix/core/node.hpp"
#include "rix/core/publisher.hpp"
#include "rix/msg/mbot/LidarScan.hpp"
#include "rix/util/cl_parser.hpp"
#include "rix/util/logger.hpp"
#include "lidar.hpp"

using Node = rix::core::Node;
using Publisher = rix::core::Publisher;
using PubImplTCP = rix::core::PubImplTCP;
using CLParser = rix::util::CLParser;

using LidarScan = rix::msg::mbot::LidarScan;

class LidarDriver {
  public:
    LidarDriver(const std::string &hub_ip, uint16_t hub_port)
    : lidar("/dev/rplidar", 115200, 700) {
        // TODO: Implement the LidarDriver constructor. This constructor should:
        //       1. Connect to the Lidar.
        //       2. Check the health of the Lidar.
        //       3. Initialize the RIX Node with the name "lidar_driver" and the
        //          hub IP and port.
        //       4. Create a publisher for LidarScan messages.
        //       5. Set the on_scan callback for the Lidar object to publish the
        //          LidarScan message. You can use a lambda function or declare 
        //          a function in this class. If you declare functions, use 
        //          std::bind to bind the member functions to 'this'. This 
        //          callback function will be invoked in the background when the
        //          driver receives a Lidar scan. Check the Lidar class for the
        //          function signature.
        //          Lambda function example:
        //              lidar.set_on_scan([this](Lidar &lidar, const LidarScan &scan) {
        //                  ...
        //              });
        //          Member function example:
        //              using std::placeholders::_1;
        //              using std::placeholders::_2;
        //              lidar.set_on_scan(std::bind(&LidarDriver::cb_func, this, _1, _2));
    }

    ~LidarDriver() {
        // TODO: Implement the LidarDriver destructor. This destructor should:
        //       1. Stop the Lidar.
    }

    void run() {
        // TODO: Implement the run method. This method should:
        //       1. Start the Lidar in non-blocking mode.
        //       2. Spin the RIX Node in blocking mode.
    }

  private:
    Node &node = Node::get_instance();
    Lidar lidar;

    // TODO: Declare a publisher for the LidarScan messages. Use a shared pointer
    //       to the Publisher class.

    // TODO: Declare the callback function to publish the LidarScan message. (if
    //       you don't want to use a lambda function).
};

int main(int argc, char **argv) {
    CLParser parser("lidar_driver", "RIX driver for the RPLidar A1");
    parser.add_opt(CLParser::Opt("ip", "i", "RIX Hub IP Address", rix::ipc::get_public_ip(), "", '1'));
    parser.parse(argc, argv);

    std::string hub_ip = parser.get_opt("ip").front();

    // TODO: Implement the main function. This program should:\
    //       1. Create a LidarDriver object with the RIX Hub IP address and the
    //          RIX Hub port.
    //       2. Run the LidarDriver object.

    return 0;
}
