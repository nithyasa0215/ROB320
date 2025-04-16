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
struct SLAMReset {
    int64_t utime;
    int32_t slam_mode;
    char slam_map_location[64];
    int8_t retain_pose;

    static inline uint32_t size() { return sizeof(SLAMReset); }

    static const SLAMReset* decode(const uint8_t *msg, size_t size) {
        if (size != SLAMReset::size()) {
            return nullptr;
        }
        return reinterpret_cast<const SLAMReset*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {6737119179256993676ULL, 2722565607918207718ULL, sizeof(SLAMReset)};
    }
};
#pragma pack(pop)

} // namespace mbot
} // namespace msg
} // namespace rix
