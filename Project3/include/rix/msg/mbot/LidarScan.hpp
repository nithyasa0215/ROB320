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
struct LidarScan {
    int64_t utime;
    int32_t num_ranges;
    float ranges[1024];
    float thetas[1024];
    float intensities[1024];
    int64_t times[1024];

    static inline uint32_t size() { return sizeof(LidarScan); }

    static const LidarScan* decode(const uint8_t *msg, size_t size) {
        if (size != LidarScan::size()) {
            return nullptr;
        }
        return reinterpret_cast<const LidarScan*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {8838870202384918899ULL, 18276289569155235428ULL, sizeof(LidarScan)};
    }
};
#pragma pack(pop)

} // namespace mbot
} // namespace msg
} // namespace rix
