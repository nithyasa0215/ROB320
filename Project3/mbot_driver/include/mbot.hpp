/**
 * @file mbot.hpp
 * @brief Header file for the MBot class, which provides an interface for controlling and communicating with the MBot robot.
 */

#pragma once

#include <sys/time.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>

#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <mutex>
#include <semaphore>
#include <string>
#include <thread>
#include <unordered_map>

/**
 * @enum MBotTopics
 * @brief Enumeration of MBot topics used for communication.
 */
enum MBotTopics {
    MBOT_TIMESYNC = 201,          // rix::msg::mbot::Timestamp
    MBOT_ODOMETRY = 210,          // rix::msg::mbot::Pose2D
    MBOT_ODOMETRY_RESET = 211,    // rix::msg::mbot::Pose2D
    MBOT_VEL_CMD = 214,           // rix::msg::mbot::Twist2D
    MBOT_IMU = 220,               // rix::msg::mbot::IMU
    MBOT_ENCODERS = 221,          // rix::msg::mbot::Encoders
    MBOT_ENCODERS_RESET = 222,    // rix::msg::mbot::Encoders
    MBOT_MOTOR_PWM_CMD = 230,     // rix::msg::mbot::MotorPWM
    MBOT_MOTOR_VEL_CMD = 231,     // rix::msg::mbot::MotorVelocity
    MBOT_MOTOR_VEL = 232,         // rix::msg::mbot::MotorVelocity
    MBOT_MOTOR_PWM = 233,         // rix::msg::mbot::MotorPWM
    MBOT_VEL = 234                // rix::msg::mbot::Twist2D
};

/**
 * @class MBot
 * @brief Class representing the MBot robot, providing methods for communication and control.
 */
class MBot {

  /**
   * @typedef Callback
   * @brief Type definition for a callback function that is called when a message is received.
   * @param MBot The MBot object that received the message.
   * @param const void* The encoded message data.
   * @param size_t The length of the message data.
   */
  using Callback = std::function<void(MBot &, const void*, size_t)>;

  public:
    /**
     * @brief Constructs an MBot object and initializes the serial port connection.
     * @param port The serial port to connect to.
     */
    MBot(const std::string &port);

    /**
     * @brief Destructor for the MBot class, ensuring proper cleanup of resources.
     */
    ~MBot();

    /**
     * @brief Sets a callback function for a specific topic.
     * @param topic_id The topic ID for which the callback is set.
     * @param callback The callback function to be called when a message for the specified topic is received.
     */
    void set_callback(uint16_t topic_id, Callback callback);

    /**
     * @brief Sends a message to a specific topic.
     * @param topic_id The topic ID to which the message is sent.
     * @param msg The message data to be sent.
     * @param msg_len The length of the message data.
     */
    void send_message(uint16_t topic_id, const void *msg, size_t msg_len);

    /**
     * @brief Starts the MBot, optionally blocking the calling thread.
     * @param block If true, the calling thread will be blocked until the MBot is stopped.
     */
    void start(bool block = true);

    /**
     * @brief Stops the MBot and joins the listener thread.
     */
    void stop();

  private:
    std::thread listener_thread; ///< Thread for listening to messages from the MBot.

    std::string port; ///< Serial port used for communication with the MBot.
    #ifdef MBOT
    struct termios options; ///< Serial port options for configuring the connection.
    #endif

    std::mutex robot_fd_mutex; ///< Mutex for protecting access to the robot file descriptor.

    int robot_fd; ///< File descriptor for the robot's serial port.
    std::unordered_map<uint16_t, Callback> callbacks; ///< Map of topic IDs to callback functions.

    bool stop_flag; ///< Flag indicating whether the listener thread should stop.

    /**
     * @brief Listener function for receiving messages from the MBot.
     */
    void listener();

    /**
     * @brief Handles a received message by invoking the appropriate callback.
     * @param topic_id The topic ID of the received message.
     * @param msg The message data.
     * @param msg_len The length of the message data.
     */
    void handle_msg(uint16_t topic_id, void *msg, size_t msg_len);

    /**
     * @brief Calculates the checksum of a message.
     * @param addends The message data for which the checksum is calculated.
     * @param len The length of the message data.
     * @return The calculated checksum.
     */
    uint8_t checksum(uint8_t *addends, int len);

    /**
     * @brief Encodes a message into a ROS packet format.
     * @param msg The message data to be encoded.
     * @param msg_len The length of the message data.
     * @param topic The topic ID of the message.
     * @param rospkt The buffer to store the encoded ROS packet.
     * @param rospkt_len The length of the ROS packet buffer.
     * @return 0 on success, -1 on failure.
     */
    int encode_msg(const uint8_t *msg, int msg_len, uint16_t topic, uint8_t *rospkt, int rospkt_len);

    /**
     * @brief Reads the header of a message from the serial port.
     * @param header_data The buffer to store the header data.
     * @return True if the header was read successfully, false otherwise.
     */
    bool read_header(uint8_t *header_data);

    /**
     * @brief Validates the header of a received message.
     * @param header_data The header data to be validated.
     * @return True if the header is valid, false otherwise.
     */
    bool validate_header(uint8_t *header_data);

    /**
     * @brief Reads a message from the serial port.
     * @param msg The buffer to store the message data.
     * @param msg_len The length of the message data.
     * @param topic_msg_data_checksum The checksum of the message data.
     * @return True if the message was read successfully, false otherwise.
     */
    bool read_message(uint8_t *msg, uint16_t msg_len, char *topic_msg_data_checksum);

    /**
     * @brief Validates a received message.
     * @param header_data The header data of the message.
     * @param msg The message data.
     * @param msg_len The length of the message data.
     * @param topic_msg_data_checksum The checksum of the message data.
     * @return True if the message is valid, false otherwise.
     */
    bool validate_message(uint8_t *header_data, uint8_t *msg, uint16_t msg_len, char topic_msg_data_checksum);
};