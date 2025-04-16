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
struct MotorVelocity {
    int64_t utime;
    float velocity[3];

    static inline uint32_t size() { return sizeof(MotorVelocity); }

    static const MotorVelocity* decode(const uint8_t *msg, size_t size) {
        if (size != MotorVelocity::size()) {
            return nullptr;
        }
        return reinterpret_cast<const MotorVelocity*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {14947313577457964899ULL, 17870851058591534250ULL, sizeof(MotorVelocity)};
    }
};
#pragma pack(pop)

} // namespace mbot
} // namespace msg
} // namespace rix
