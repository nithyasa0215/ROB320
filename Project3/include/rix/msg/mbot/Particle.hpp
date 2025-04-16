#pragma once

#include <sys/types.h>

#include <string>
#include <cstring>

#include "rix/msg/common.hpp"
#include "rix/msg/component/MessageInfo.hpp"

#include "rix/msg/mbot/Pose2D.hpp"

namespace rix {
namespace msg {
namespace mbot {

#pragma pack(push, 1)
struct Particle {
    mbot::Pose2D pose;
    mbot::Pose2D parent_pose;
    double weight;

    static inline uint32_t size() { return sizeof(Particle); }

    static const Particle* decode(const uint8_t *msg, size_t size) {
        if (size != Particle::size()) {
            return nullptr;
        }
        return reinterpret_cast<const Particle*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {2579152085338388704ULL, 13782723282460634354ULL, sizeof(Particle)};
    }
};
#pragma pack(pop)

} // namespace mbot
} // namespace msg
} // namespace rix
