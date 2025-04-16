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
template <uint32_t N>
struct String {
    char data[N];

    static inline uint32_t size() { return sizeof(String<N>); }

    static const String* decode(const uint8_t *msg, size_t size) {
        if (size != String::size()) {
            return nullptr;
        }
        return reinterpret_cast<const String*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {7028215995610633827ULL, 12240547462482537013ULL, sizeof(String)};
    }
};
#pragma pack(pop)

} // namespace standard
} // namespace msg
} // namespace rix
