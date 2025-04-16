#pragma once

#include <sys/types.h>

#include <string>
#include <cstring>

#include "rix/msg/common.hpp"
#include "rix/msg/component/MessageInfo.hpp"


namespace rix {
namespace msg {
namespace standard {

#pragma pack(push, 1)
struct Time {
    int32_t sec;
    int32_t nsec;

    static inline uint32_t size() { return sizeof(Time); }

    static const Time* decode(const uint8_t *msg, size_t size) {
        if (size != Time::size()) {
            return nullptr;
        }
        return reinterpret_cast<const Time*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {6130763214921892067ULL, 10143670270205710718ULL, sizeof(Time)};
    }
};
#pragma pack(pop)

} // namespace standard
} // namespace msg
} // namespace rix
