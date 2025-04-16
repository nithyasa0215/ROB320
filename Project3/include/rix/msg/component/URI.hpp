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
struct URI {
    uint16_t port;
    char address[16];

    static inline uint32_t size() { return sizeof(URI); }

    static const URI* decode(const uint8_t *msg, size_t size) {
        if (size != URI::size()) {
            return nullptr;
        }
        return reinterpret_cast<const URI*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {17439838763354272268ULL, 11634972329034599865ULL, sizeof(URI)};
    }
};
#pragma pack(pop)

} // namespace component
} // namespace msg
} // namespace rix
