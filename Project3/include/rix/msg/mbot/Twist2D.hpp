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
struct Twist2D {
    int64_t utime;
    float vx;
    float vy;
    float wz;

    static inline uint32_t size() { return sizeof(Twist2D); }

    static const Twist2D* decode(const uint8_t *msg, size_t size) {
        if (size != Twist2D::size()) {
            return nullptr;
        }
        return reinterpret_cast<const Twist2D*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {12166359628271503854ULL, 8186079126154682043ULL, sizeof(Twist2D)};
    }
};
#pragma pack(pop)

} // namespace mbot
} // namespace msg
} // namespace rix
