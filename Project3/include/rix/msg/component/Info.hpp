#pragma once

#include <sys/types.h>

#include <string>
#include <cstring>

#include "rix/msg/common.hpp"
#include "rix/msg/component/MessageInfo.hpp"

#include "rix/msg/component/ComponentInfo.hpp"
#include "rix/msg/component/ID.hpp"

namespace rix {
namespace msg {
namespace component {

#pragma pack(push, 1)
struct Info {
    component::ComponentInfo component_info;
    component::ID contact_id;
    uint8_t opcode;
    uint8_t error;

    static inline uint32_t size() { return sizeof(Info); }

    static const Info* decode(const uint8_t *msg, size_t size) {
        if (size != Info::size()) {
            return nullptr;
        }
        return reinterpret_cast<const Info*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {10556714876289969800ULL, 8757234750775443487ULL, sizeof(Info)};
    }
};
#pragma pack(pop)

} // namespace component
} // namespace msg
} // namespace rix
