#pragma once

#include <sys/types.h>

#include <string>
#include <cstring>

#include "rix/msg/common.hpp"
#include "rix/msg/component/MessageInfo.hpp"

#include "rix/msg/standard/Time.hpp"

namespace rix {
namespace msg {
namespace standard {

#pragma pack(push, 1)
struct Header {
    uint32_t seq;
    standard::Time stamp;
    char frame_id[32];

    static inline uint32_t size() { return sizeof(Header); }

    static const Header* decode(const uint8_t *msg, size_t size) {
        if (size != Header::size()) {
            return nullptr;
        }
        return reinterpret_cast<const Header*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {4542894773782276688ULL, 8045714310799764125ULL, sizeof(Header)};
    }
};
#pragma pack(pop)

} // namespace standard
} // namespace msg
} // namespace rix
