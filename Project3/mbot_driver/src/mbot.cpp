#include "mbot.hpp"

#include "rix/core/common.hpp"
#include "rix/msg/mbot/Pose2D.hpp"
#include "rix/msg/mbot/Twist2D.hpp"
#include "rix/msg/mbot/Encoders.hpp"
#include "rix/msg/mbot/IMU.hpp"
#include "rix/msg/mbot/MotorPWM.hpp"
#include "rix/msg/mbot/MotorVelocity.hpp"
#include "rix/msg/mbot/Timestamp.hpp"

#define SYNC_FLAG 0xff                                         /**< Sync flag for ROS packets */
#define VERSION_FLAG 0xfe                                      /**< Version flag for ROS packets */
#define ROS_HEADER_LENGTH 7                                    /**< Length of the ROS packet header */
#define ROS_FOOTER_LENGTH 1                                    /**< Length of the ROS packet footer */
#define ROS_PKG_LENGTH (ROS_HEADER_LENGTH + ROS_FOOTER_LENGTH) /**< Length of the ROS packet overhead */

MBot::MBot(const std::string &port)
    : robot_fd(-1),
      port(port),
      stop_flag(false) {
    #ifdef MBOT
    robot_fd = open(this->port.c_str(), O_RDWR);
    if (robot_fd < 0) {
        std::cerr << "Error, cannot open the serial port." << std::endl;
        exit(-1);
    }
    // Set up the serial port
    tcgetattr(robot_fd, &options);
    cfsetspeed(&options, B115200);
    options.c_cflag &= ~(CSIZE | PARENB | CSTOPB | CRTSCTS);
    options.c_cflag |= CS8 | CREAD | CLOCAL;
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ICANON | ISIG | ECHO | IEXTEN); /* Set non-canonical mode */
    options.c_cc[VTIME] = 1;
    options.c_cc[VMIN] = 0;
    cfmakeraw(&options);
    tcflush(robot_fd, TCIFLUSH);
    tcsetattr(robot_fd, TCSANOW, &options);
    if (tcgetattr(robot_fd, &options) != 0) {
        close(robot_fd);
        exit(-1);
    }
    #endif
}

MBot::~MBot() {
    stop();
    #ifdef MBOT
    close(robot_fd);
    #endif
}

void MBot::set_callback(uint16_t topic_id, Callback callback) {
    callbacks[topic_id] = callback;
}

void MBot::send_message(uint16_t topic_id, const void *msg, size_t msg_len) {
    #ifdef MBOT
    // Encode the message
    const size_t rospkt_len = msg_len + ROS_PKG_LENGTH;
    uint8_t *rospkt = new uint8_t[rospkt_len];
    encode_msg(static_cast<const uint8_t*>(msg), msg_len, topic_id, rospkt, rospkt_len);

    // Send the message
    robot_fd_mutex.lock();
    ssize_t bytes_sent = 0;
    while (bytes_sent < rospkt_len) {
        ssize_t status = write(robot_fd, rospkt + bytes_sent, rospkt_len - bytes_sent);
        if (status < 0) {
            perror("write");
            break;
        }
        bytes_sent += status;
    }
    robot_fd_mutex.unlock();

    delete[] rospkt;
    #else
    Logger::log<LogLevel::INFO>("Received message. topic_id: %u, msg_len: %lu", topic_id, msg_len);
    #endif
}

void MBot::start(bool block) {
    stop(); // Ensure all threads have been joined
    stop_flag = false;

    // Start listener thread
    listener_thread = std::thread(&MBot::listener, this);

    if (block) {
        listener_thread.join();
    }
}

void MBot::stop() {
    stop_flag = true;
    if (listener_thread.get_id() != std::this_thread::get_id() && listener_thread.joinable()) {
        listener_thread.join();
    }
}

#ifdef MBOT
void MBot::listener() {
    uint8_t header_data[ROS_HEADER_LENGTH];
    header_data[0] = 0x00;

    while (!stop_flag) {
        // Read the header and check if we lost serial connection
        int header_status = read_header(header_data);
        if (header_status < 0) {
            std::cerr << "[ERROR] Serial device is not available" << std::endl;
            break;
        }

        bool valid_header = (header_status == 1);
        if (valid_header) {
            valid_header = validate_header(header_data);
        }

        if (valid_header) {
            uint16_t msg_len = ((uint16_t)header_data[3] << 8) + (uint16_t)header_data[2];
            uint16_t topic_id = ((uint16_t)header_data[6] << 8) + (uint16_t)header_data[5];
            uint8_t *msg = new uint8_t[msg_len];

            int avail = 0;
            ioctl(robot_fd, FIONREAD, &avail);
            while (avail < (msg_len + 1) && !stop_flag) {
                usleep(1000);
                ioctl(robot_fd, FIONREAD, &avail);
            }
            if (stop_flag) {
                break;
            }

            // Read the message and check if we lost serial connection
            char topic_msg_data_checksum = 0;
            int message_status = read_message(msg, msg_len, &topic_msg_data_checksum);
            if (message_status < 0) {
                std::cerr << "[ERROR] Serial device is not available" << std::endl;
                break;
            }

            bool valid_message = (message_status == 1);
            if (valid_message) {
                valid_message = validate_message(header_data, msg, msg_len, topic_msg_data_checksum);
                if (valid_message) {
                    handle_msg(topic_id, msg, msg_len);
                }
            }
            delete[] msg;
        }

        header_data[0] = 0x00;
    }
}
#else

void MBot::listener() {
/*
    enum MBotTopics {
        MBOT_ODOMETRY = 210, 
        MBOT_IMU = 220,
        MBOT_ENCODERS = 221,
        MBOT_MOTOR_VEL = 232,
        MBOT_MOTOR_PWM = 233,
        MBOT_VEL = 234
    };
*/
    while (!stop_flag) {
        rix::msg::mbot::Pose2D pose;
        pose.x = 1 + (rand() % 10);
        pose.y = 1 + (rand() % 10);
        pose.theta = 1 + (rand() % 10);
        pose.utime = 1 + (rand() % 10);
        handle_msg(MBOT_ODOMETRY, &pose, sizeof(pose));
        
        rix::msg::mbot::IMU imu;
        imu.utime = 1 + (rand() % 10);
        for (int i = 0; i < 3; i++) {
            imu.gyro[i] = 1 + (rand() % 10);
            imu.accel[i] = 1 + (rand() % 10);
            imu.mag[i] = 1 + (rand() % 10);
            imu.angles_rpy[i] = 1 + (rand() % 10);
        }
        for (int i = 0; i < 4; i++) {
            imu.angles_quat[i] = 1 + (rand() % 10);
        }
        imu.temp = 1 + (rand() % 10);
        handle_msg(MBOT_IMU, &imu, sizeof(imu));

        rix::msg::mbot::Twist2D twist2d;
        twist2d.vx = 1 + (rand() % 10);
        twist2d.vy = 1 + (rand() % 10);
        twist2d.wz = 1 + (rand() % 10);
        twist2d.utime = 1 + (rand() % 10);
        handle_msg(MBOT_VEL, &twist2d, sizeof(twist2d));

        rix::msg::mbot::Encoders encoders;
        encoders.utime = 1 + (rand() % 10);
        encoders.delta_time = 1 + (rand() % 10);
        for (int i = 0; i < 2; i++) {
            encoders.delta_ticks[i] = 1 + (rand() % 10);
            encoders.ticks[i] = 1 + (rand() % 10);
        }
        handle_msg(MBOT_ENCODERS, &encoders, sizeof(encoders));

        rix::msg::mbot::MotorPWM motor_pwm;
        motor_pwm.utime = 1 + (rand() % 10);
        motor_pwm.pwm[0] = 1 + (rand() % 10);
        motor_pwm.pwm[1] = 1 + (rand() % 10);
        motor_pwm.pwm[2] = 1 + (rand() % 10);   
        handle_msg(MBOT_MOTOR_PWM, &motor_pwm, sizeof(motor_pwm));

        rix::msg::mbot::MotorVelocity motor_velocity;
        motor_velocity.utime = 1 + (rand() % 10);
        motor_velocity.velocity[0] = 1 + (rand() % 10);
        motor_velocity.velocity[1] = 1 + (rand() % 10);
        motor_velocity.velocity[2] = 1 + (rand() % 10);
        handle_msg(MBOT_MOTOR_VEL, &motor_velocity, sizeof(motor_velocity));

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
#endif

void MBot::handle_msg(uint16_t topic_id, void *msg, size_t msg_len) {
    auto it = callbacks.find(topic_id);
    if (it != callbacks.end()) {
        it->second(*this, msg, msg_len);
    }
}

uint8_t MBot::checksum(uint8_t *addends, int len) {
    // takes in an array and sums the contents then checksums the array
    int sum = 0;
    for (int i = 0; i < len; i++) {
        sum += addends[i];
    }
    return 255 - ((sum) % 256);
}

int MBot::encode_msg(const uint8_t *msg, int msg_len, uint16_t topic, uint8_t *rospkt, int rospkt_len) {
    // SANITY CHECKS
    if (msg_len + ROS_PKG_LENGTH != rospkt_len) {
        return -1;
    }

    // CREATE ROS PACKET
    // for ROS protocol and packet format see link: http://wiki.ros.org/rosserial/Overview/Protocol
    rospkt[0] = SYNC_FLAG;
    rospkt[1] = VERSION_FLAG;
    rospkt[2] = (uint8_t)(msg_len & 0xFF);  // message length lower 8/16b via bitwise AND and cast
    rospkt[3] = (uint8_t)(msg_len >> 8);    // message length higher 8/16b via bitshift and cast

    uint8_t cs1_addends[2] = {rospkt[2], rospkt[3]};
    rospkt[4] = checksum(cs1_addends, 2);  // checksum over message length
    rospkt[5] = (uint8_t)(topic & 0xFF);   // message topic lower 8/16b via bitwise AND and cast
    rospkt[6] = (uint8_t)(topic >> 8);     // message length higher 8/16b via bitshift and cast

    memcpy(&rospkt[ROS_HEADER_LENGTH], msg, msg_len);  // copy message data to packet

    uint8_t cs2_addends[msg_len + 2];  // create array for the checksum over topic and message content
    cs2_addends[0] = rospkt[5];
    cs2_addends[1] = rospkt[6];
    for (int i = 0; i < msg_len; i++) {
        cs2_addends[i + 2] = msg[i];
    }

    rospkt[rospkt_len - 1] = checksum(cs2_addends, msg_len + 2);  // checksum over message data and topic

    return 0;
}

bool MBot::read_header(uint8_t *header_data) {
    unsigned char trigger_val = 0x00;
    int rc = 0x00;
    while (trigger_val != 0xff && !stop_flag && rc != 1) {
        rc = read(robot_fd, &trigger_val, 1);
        if (rc < 0) {
            return -1;
        }
    }
    header_data[0] = trigger_val;

    rc = read(robot_fd, &header_data[1], ROS_HEADER_LENGTH - 1);
    if (rc < 0) {
        return -1;
    }

    return (rc == ROS_HEADER_LENGTH - 1);
}

// Header validation function
bool MBot::validate_header(uint8_t *header_data) {
    bool valid_header = (header_data[1] == 0xfe);
    uint8_t cs1_addends[2] = {header_data[2], header_data[3]};
    uint8_t cs_msg_len = checksum(cs1_addends, 2);
    valid_header = valid_header && (cs_msg_len == header_data[4]);

    return valid_header;
}

// Message read function
bool MBot::read_message(uint8_t *msg, uint16_t msg_len, char *topic_msg_data_checksum) {
    int rc = read(robot_fd, msg, msg_len);
    if (rc < 0) {
        return -1;
    }
    bool valid_message = (rc == msg_len);

    rc = read(robot_fd, topic_msg_data_checksum, 1);
    if (rc < 0) {
        return -1;
    }
    valid_message = valid_message && (rc == 1);

    return valid_message;
}

// Message validation function
bool MBot::validate_message(uint8_t *header_data, uint8_t *msg, uint16_t msg_len, char topic_msg_data_checksum) {
    uint8_t cs2_addends[msg_len + 2];
    cs2_addends[0] = header_data[5];
    cs2_addends[1] = header_data[6];
    for (int i = 0; i < msg_len; i++) {
        cs2_addends[i + 2] = msg[i];
    }

    uint8_t cs_topic_msg_data = checksum(cs2_addends, msg_len + 2);
    bool valid_message = (cs_topic_msg_data == topic_msg_data_checksum);

    return valid_message;
}
