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
struct SLAMStatus {
    int64_t utime;
    int32_t slam_mode;
    char map_path[32];

    static inline uint32_t size() { return sizeof(SLAMStatus); }

    static const SLAMStatus* decode(const uint8_t *msg, size_t size) {
        if (size != SLAMStatus::size()) {
            return nullptr;
        }
        return reinterpret_cast<const SLAMStatus*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {10624977162770990104ULL, 2395151120057526879ULL, sizeof(SLAMStatus)};
    }
};
#pragma pack(pop)

} // namespace mbot
} // namespace msg
} // namespace rix
