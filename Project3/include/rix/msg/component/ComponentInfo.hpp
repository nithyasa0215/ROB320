#pragma once

#include <sys/types.h>

#include <string>
#include <cstring>

#include "rix/msg/common.hpp"
#include "rix/msg/component/MessageInfo.hpp"


namespace rix {
namespace msg {
namespace component {

#pragma pack(push, 1)
struct ComponentInfo {
    component::MessageInfo message_info[3];
    uint64_t node_id;
    uint64_t component_id;
    uint64_t machine_id;
    uint8_t protocol;
    char topic[32];

    static inline uint32_t size() { return sizeof(ComponentInfo); }

    static const ComponentInfo* decode(const uint8_t *msg, size_t size) {
        if (size != ComponentInfo::size()) {
            return nullptr;
        }
        return reinterpret_cast<const ComponentInfo*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {12512736207596013427ULL, 6073665220113294299ULL, sizeof(ComponentInfo)};
    }
};
#pragma pack(pop)

} // namespace component
} // namespace msg
} // namespace rix
