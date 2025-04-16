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
struct MotorPWM {
    int64_t utime;
    float pwm[3];

    static inline uint32_t size() { return sizeof(MotorPWM); }

    static const MotorPWM* decode(const uint8_t *msg, size_t size) {
        if (size != MotorPWM::size()) {
            return nullptr;
        }
        return reinterpret_cast<const MotorPWM*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {10778092090436055129ULL, 17757085834716293150ULL, sizeof(MotorPWM)};
    }
};
#pragma pack(pop)

} // namespace mbot
} // namespace msg
} // namespace rix
