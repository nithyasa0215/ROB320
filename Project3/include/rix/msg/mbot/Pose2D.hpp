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
struct Pose2D {
    int64_t utime;
    float x;
    float y;
    float theta;

    static inline uint32_t size() { return sizeof(Pose2D); }

    static const Pose2D* decode(const uint8_t *msg, size_t size) {
        if (size != Pose2D::size()) {
            return nullptr;
        }
        return reinterpret_cast<const Pose2D*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {13219665431448891103ULL, 1464484153955705328ULL, sizeof(Pose2D)};
    }
};
#pragma pack(pop)

} // namespace mbot
} // namespace msg
} // namespace rix
