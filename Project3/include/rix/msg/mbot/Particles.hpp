#pragma once

#include <sys/types.h>

#include <string>
#include <cstring>

#include "rix/msg/common.hpp"
#include "rix/msg/component/MessageInfo.hpp"

#include "rix/msg/mbot/Particle.hpp"

namespace rix {
namespace msg {
namespace mbot {

#pragma pack(push, 1)
template <uint32_t N>
struct Particles {
    int64_t utime;
    uint32_t num_particles;
    mbot::Particle particles[N];

    static inline uint32_t size() { return sizeof(Particles<N>); }

    static const Particles* decode(const uint8_t *msg, size_t size) {
        if (size != Particles::size()) {
            return nullptr;
        }
        return reinterpret_cast<const Particles*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {12181471296360667854ULL, 9189011070366304011ULL, sizeof(Particles)};
    }
};
#pragma pack(pop)

} // namespace mbot
} // namespace msg
} // namespace rix
