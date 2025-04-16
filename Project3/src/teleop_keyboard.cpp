// ************************************************************************** //
// * This file is a simple teleop node that publishes Twist2D messages      * //
// * based on keyboard input. This file contains code that will help you    * //
// * understand how to use the rix library.                                 * //
// ************************************************************************** //

#include <termios.h>
#include <unistd.h>

#include <iostream>
#include <memory>
#include <string>
#include <csignal>
#include <unordered_map>

#include "rix/core/node.hpp"
#include "rix/core/publisher.hpp"
#include "rix/msg/mbot/Twist2D.hpp"
#include "rix/util/timing.hpp"
#include "rix/util/cl_parser.hpp"
#include "rix/util/logger.hpp"

const char* control_msg = "Reading from the keyboard  and Publishing to Twist!\n"
                          "---------------------------\n"
                          "Moving around:\n"
                          "   u    i    o\n"
                          "   j    k    l\n"
                          "q/z : increase/decrease max speeds by 10%\n"
                          "w/x : increase/decrease only linear speed by 10%\n"
                          "e/c : increase/decrease only angular speed by 10%\n"
                          "anything else : stop\n"
                          "CTRL-C to quit";

using SignalHandler = rix::util::SignalHandler;
using CLParser = rix::util::CLParser;
using Node = rix::core::Node;
using Publisher = rix::core::Publisher;
using PubImplTCP = rix::core::PubImplTCP;
using Twist2D = rix::msg::mbot::Twist2D;

struct termios orig_termios;

bool ctrl_c_pressed = false;

std::unordered_map<char, Twist2D> move_bindings = {
    {'i', {0, 1.0, 0.0, 0.0}},
    {'k', {0, -1.0, 0.0, 0.0}},
    {'j', {0, 0.0, 1.0, 0.0}},
    {'l', {0, 0.0, -1.0, 0.0}},
    {'u', {0, 0.0, 0.0, 1.0}},
    {'o', {0, 0.0, 0.0, -1.0}}
};

std::unordered_map<char, std::pair<float, float>> speed_bindings = {
    {'q', {1.1, 1.1}},
    {'z', {0.9, 0.9}},
    {'w', {1.1, 1.0}},
    {'x', {0.9, 1.0}},
    {'e', {1.0, 1.1}},
    {'c', {1.0, 0.9}}
};

class TerminalSettings {
public:
    TerminalSettings() {
        tcgetattr(STDIN_FILENO, &orig_termios);
        struct termios newt = orig_termios;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }

    ~TerminalSettings() {
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
    }
};

void handle_sigint(int sig) {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
    ctrl_c_pressed = true;
}

int main(int argc, char **argv) {
    CLParser parser("teleop_keyboard", "Publishes twist messages from keyboard");
    parser.add_opt(CLParser::Opt("ip", "i", "RIX Hub IP Address", rix::ipc::get_public_ip(), "", '1'));
    parser.add_opt(CLParser::Opt("freq", "f", "Publish rate in Hz", "25", "", '1'));
    parser.add_opt(CLParser::Opt("lin_speed", "l", "Initial linear speed (m/s)", "0.5", "", '1'));
    parser.add_opt(CLParser::Opt("ang_speed", "a", "Initial angular speed (rad/s)", "1.57", "", '1'));
    parser.add_opt(CLParser::Opt("lin_limit", "", "Linear speed limit", "1.0", "", '1'));
    parser.add_opt(CLParser::Opt("ang_limit", "", "Angular speed limit", "3.14", "", '1'));
    parser.parse(argc, argv);

    std::string hub_ip = parser.get_opt("ip").front();
    double freq = std::stod(parser.get_opt("freq").front());
    float lin_speed = std::stof(parser.get_opt("lin_speed").front());
    float ang_speed = std::stof(parser.get_opt("ang_speed").front());
    float lin_limit = std::stof(parser.get_opt("lin_limit").front());
    float ang_limit = std::stof(parser.get_opt("ang_limit").front());

    Node &node = Node::get_instance();
    node.init("teleop_keyboard", hub_ip, RIX_HUB_PORT);

    SignalHandler &handler = SignalHandler::get_instance();
    handler.register_handle(SIGINT, handle_sigint);

    std::shared_ptr<Publisher> pub = node.advertise<Twist2D, PubImplTCP>("robot_vel_cmd");
    if (!pub) {
        Logger::log<LogLevel::ERROR>("Failed to create publisher");
        return -1;
    }

    node.spin(false);

    rix::util::Rate rate(freq);

    // Ensure terminal settings are reset properly
    TerminalSettings terminal_settings;

    Logger::log<LogLevel::INFO>(control_msg);

    Twist2D cmd;
    while (!ctrl_c_pressed) {
        char input = getchar();
        if (input < 0) {
            return 0;
        }
        if (speed_bindings.find(input) != speed_bindings.end()) {
            auto [lin, ang] = speed_bindings[input];
            lin_speed = std::min(lin_speed * lin, lin_limit);
            ang_speed = std::min(ang_speed * ang, ang_limit);
            Logger::log<LogLevel::INFO>("Linear speed: %.2f, Angular speed: %.2f", lin_speed, ang_speed);
        } else if (move_bindings.find(input) != move_bindings.end()) {
            cmd = move_bindings[input];
        } else {
            cmd = {0, 0, 0, 0};
        }

        Twist2D twist;
        twist.vx = cmd.vx * lin_speed;
        twist.vy = cmd.vy * lin_speed;
        twist.wz = cmd.wz * ang_speed;
        pub->publish(twist);
        Logger::log<LogLevel::INFO>("Published: vx = %.2f, vy = %.2f, wz = %.2f", twist.vx, twist.vy, twist.wz);
        rate.sleep();
    }

    return 0;
}
