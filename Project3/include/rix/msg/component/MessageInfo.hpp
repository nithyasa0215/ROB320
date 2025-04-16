#pragma once

#include <sys/types.h>

#include <string>
#include <cstring>

#include "rix/msg/common.hpp"

namespace rix {
namespace msg {
namespace component {

#pragma pack(push, 1)
struct MessageInfo {
    uint64_t hash_upper;
    uint64_t hash_lower;
    uint32_t length;

    static inline uint32_t size() { return sizeof(MessageInfo); }

    static const MessageInfo* decode(const uint8_t *msg, size_t size) {
        if (size != MessageInfo::size()) {
            return nullptr;
        }
        return reinterpret_cast<const MessageInfo*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {16082948690852741757ULL, 15538079429110084989ULL, sizeof(MessageInfo)};
    }
};
#pragma pack(pop)

} // namespace component
} // namespace msg
} // namespace rix
