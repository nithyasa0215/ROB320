#pragma once

#include <sys/types.h>

#include <string>
#include <cstring>

#include "rix/msg/common.hpp"
#include "rix/msg/component/MessageInfo.hpp"


namespace rix {
namespace msg {
namespace mbot {

#pragma pack(push, 1)
struct IMU {
    int64_t utime;
    float gyro[3];
    float accel[3];
    float mag[3];
    float angles_rpy[3];
    float angles_quat[4];
    float temp;

    static inline uint32_t size() { return sizeof(IMU); }

    static const IMU* decode(const uint8_t *msg, size_t size) {
        if (size != IMU::size()) {
            return nullptr;
        }
        return reinterpret_cast<const IMU*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {14119914786346351890ULL, 8439826378632525256ULL, sizeof(IMU)};
    }
};
#pragma pack(pop)

} // namespace mbot
} // namespace msg
} // namespace rix
