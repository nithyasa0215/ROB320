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
struct Timestamp {
    int64_t utime;

    static inline uint32_t size() { return sizeof(Timestamp); }

    static const Timestamp* decode(const uint8_t *msg, size_t size) {
        if (size != Timestamp::size()) {
            return nullptr;
        }
        return reinterpret_cast<const Timestamp*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {781326455680732794ULL, 902744533725469830ULL, sizeof(Timestamp)};
    }
};
#pragma pack(pop)

} // namespace mbot
} // namespace msg
} // namespace rix
