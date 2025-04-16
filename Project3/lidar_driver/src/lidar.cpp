#include "lidar.hpp"

#define PI 3.1415926535f
#define LIDAR_SCAN_SIZE 8192

Lidar::Lidar(const std::string &port, uint32_t baudrate, uint16_t pwm)
: is_connected(false), port(port), baudrate(baudrate), pwm(pwm) {
    std::cout << "Lidar::Lidar called." << std::endl;
    #ifdef MBOT
    drv = RPlidarDriver::CreateDriver();
    if (!drv) {
        std::cerr << "Error, cannot create driver." << std::endl;
        exit(-1);
    }
    #endif
    std::cout << "Lidar::Lidar returning." << std::endl;
}

Lidar::~Lidar() {
    stop();
    #ifdef MBOT
    if (drv) {
        RPlidarDriver::DisposeDriver(drv);
    }
    #endif
}

void Lidar::set_on_scan(std::function<void(Lidar&, const LidarScan&)> on_scan) { this->on_scan = on_scan; }

int Lidar::connect() {
    is_connected = false;
    #ifdef MBOT
    if (IS_FAIL(drv->connect(port.c_str(), baudrate))) {
        std::cerr << "Error, cannot bind to the serial port /dev/rplidar." << std::endl;
        return -1;
    }
    #endif
    is_connected = true;
    return 0;
}

int Lidar::check_health() {
    #ifdef MBOT
    rplidar_response_device_info_t devinfo;

    u_result op_result = drv->getDeviceInfo(devinfo);
    if (IS_FAIL(op_result)) {
        std::cerr << "Error, cannot get device info." << std::endl;
        return -1;
    }

    rplidar_response_device_health_t healthinfo;
    op_result = drv->getHealth(healthinfo);
    if (!IS_OK(op_result)) {
        std::cerr << "Error, cannot retrieve the lidar health code: " << op_result << std::endl;
        return -1;
    }
    if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
        std::cerr << "Error, rplidar internal error detected. Please reboot the device to retry." << std::endl;
        return -1;
    }
    #endif
    return 0;
}

int Lidar::start(bool block) {
    if (!is_connected) {
        return -1;
    }

    stop();  // Ensure thread has been joined
    stop_flag = false;
    run_thread = std::thread(&Lidar::run, this);
    if (block) {
        run_thread.join();
    }
    return 0;
}

void Lidar::stop() {
    stop_flag = true;
    if (run_thread.get_id() != std::this_thread::get_id() && run_thread.joinable()) {
        run_thread.join();
    }
}

#ifdef MBOT
void Lidar::run() {
    drv->startMotor();
    drv->setMotorPWM(pwm);
    drv->startScan(false, true);

    int64_t now = utime_now();
    int64_t prev_time;

    rplidar_response_measurement_node_hq_t nodes[LIDAR_SCAN_SIZE];
    while (is_connected && !stop_flag) {
        size_t recv_size = LIDAR_SCAN_SIZE;
        std::memset(&nodes, 0, sizeof(nodes));
        u_result op_result = drv->grabScanDataHq(nodes, recv_size);
        if (IS_OK(op_result)) {
            std::memset(&current_scan, 0, sizeof(current_scan));
            prev_time = now;
            now = utime_now();  // get current timestamp in milliseconds
            int64_t delta = (now - prev_time) / LIDAR_SCAN_SIZE;
            drv->ascendScanData(nodes, LIDAR_SCAN_SIZE);
            current_scan.utime = now;
            int32_t num_ranges = 0;
            for (int i = 0; i < LIDAR_SCAN_SIZE; i++) {
                int scan_idx = LIDAR_SCAN_SIZE - i - 1;
                float range = nodes[scan_idx].dist_mm_q2 / 4.0f / 1000.0f;
                if (range < 0.1 || range > 6.0) {
                    continue;
                }
                current_scan.ranges[num_ranges] = range;
                current_scan.thetas[num_ranges] = 2 * PI - nodes[scan_idx].angle_z_q14 * (PI / 32768.0);
                current_scan.intensities[num_ranges] = nodes[scan_idx].quality >> RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT;
                current_scan.times[num_ranges] = prev_time + i * delta;
                num_ranges++;
                if (num_ranges >= 1024) {
                    break;
                }
            }
            current_scan.num_ranges = num_ranges;
            if (on_scan) {
                on_scan(*this, current_scan);
            }
        } else {
            if (connect()) {
                drv->startMotor();
                drv->setMotorPWM(pwm);
                drv->startScan(false, true);
            }
        }
    }
    drv->stop();
    drv->stopMotor();
}
#else
void Lidar::run() {
    int64_t now = utime_now();
    int64_t prev_time;

    const int num_ranges = sizeof(current_scan.ranges) / sizeof(current_scan.ranges[0]);

    while (is_connected && !stop_flag) {
        std::memset(&current_scan, 0, sizeof(current_scan));
        prev_time = now;
        now = utime_now();  // get current timestamp in milliseconds
        int64_t delta = (now - prev_time) / num_ranges;
        current_scan.utime = now;

        for (int i = 0; i < num_ranges; i++) {
            float range = 0.1 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (6.0 - 0.1)));
            float angle = i * (2 * PI / num_ranges);
            current_scan.ranges[i] = range;
            current_scan.thetas[i] = angle;
            current_scan.intensities[i] = 1;
            current_scan.times[i] = prev_time + i * delta;
        }
        current_scan.num_ranges = num_ranges;
        if (on_scan) {
            on_scan(*this, current_scan);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
#endif

int64_t Lidar::utime_now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}
