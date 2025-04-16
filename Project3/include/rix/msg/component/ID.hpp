#pragma once

#include <sys/types.h>

#include <string>
#include <cstring>

#include "rix/msg/common.hpp"
#include "rix/msg/component/MessageInfo.hpp"

#include "rix/msg/component/URI.hpp"

namespace rix {
namespace msg {
namespace component {

#pragma pack(push, 1)
struct ID {
    uint64_t component_id;
    component::URI uri;

    static inline uint32_t size() { return sizeof(ID); }

    static const ID* decode(const uint8_t *msg, size_t size) {
        if (size != ID::size()) {
            return nullptr;
        }
        return reinterpret_cast<const ID*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {8664501893790460407ULL, 723159008502827223ULL, sizeof(ID)};
    }
};
#pragma pack(pop)

} // namespace component
} // namespace msg
} // namespace rix
