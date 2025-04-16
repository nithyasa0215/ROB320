#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include <cstddef>
#include <cstring>
#include <functional>
#include <iostream>
#include <thread>

#include "rix/msg/mbot/LidarScan.hpp"

#ifdef MBOT
#include <rplidar.h>
using namespace rp::standalone::rplidar;
#endif
using LidarScan = rix::msg::mbot::LidarScan;

/**
 * @class Lidar
 * @brief Manages the connection and data acquisition from an RPLIDAR device.
 */
class Lidar {
  public:
    /**
     * @brief Constructs a new Lidar object.
     * @param port The serial port to which the LIDAR is connected.
     * @param baudrate The baud rate for the serial connection.
     * @param pwm The PWM value for the LIDAR motor.
     */
    Lidar(const std::string &port, uint32_t baudrate, uint16_t pwm);

    /**
     * @brief Destroys the Lidar object.
     */
    ~Lidar();

    /**
     * @brief Sets the callback function to be called when a scan is received.
     * @param on_scan The callback function.
     */
    void set_on_scan(std::function<void(Lidar &, const LidarScan &)> on_scan);

    /**
     * @brief Connects to the LIDAR device.
     * @return 0 if the connection was successful, -1 otherwise.
     */
    int connect();

    /**
     * @brief Checks the health of the LIDAR device.
     * @return 0 if the device is healthy, -1 otherwise.
     */
    int check_health();

    /**
     * @brief Starts the LIDAR scanning process.
     * @param block If true, the function blocks until the scanning process is stopped.
     * @return 0 if the scanning process was started successfully, -1 otherwise.
     */
    int start(bool block = true);

    /**
     * @brief Stops the LIDAR scanning process.
     */
    void stop();

  private:
    std::thread run_thread;  ///< The thread running the LIDAR scanning process.
    std::string port;  ///< The serial port to which the LIDAR is connected.

    #ifdef MBOT
    RPlidarDriver *drv;  ///< The RPLIDAR driver.
    #endif

    LidarScan current_scan;  ///< The current LIDAR scan data.
    std::function<void(Lidar &, const LidarScan &)> on_scan;  ///< The callback function for scan data.
    uint32_t baudrate;  ///< The baud rate for the serial connection.
    uint16_t pwm;  ///< The PWM value for the LIDAR motor.
    bool is_connected;  ///< Indicates if the LIDAR is connected.
    bool stop_flag;  ///< Indicates if the scanning process should be stopped.

    /**
     * @brief Runs the LIDAR scanning process.
     */
    void run();

    /**
     * @brief Gets the current time in microseconds.
     * @return The current time in microseconds.
     */
    int64_t utime_now();
};