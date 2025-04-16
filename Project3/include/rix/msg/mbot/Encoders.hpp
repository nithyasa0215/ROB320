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
struct Encoders {
    int64_t utime;
    int64_t ticks[3];
    int32_t delta_ticks[3];
    int32_t delta_time;

    static inline uint32_t size() { return sizeof(Encoders); }

    static const Encoders* decode(const uint8_t *msg, size_t size) {
        if (size != Encoders::size()) {
            return nullptr;
        }
        return reinterpret_cast<const Encoders*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {4859274432725272955ULL, 3787616311786401264ULL, sizeof(Encoders)};
    }
};
#pragma pack(pop)

} // namespace mbot
} // namespace msg
} // namespace rix
