#pragma once

#include <sys/types.h>

#include <string>
#include <cstring>

#include "rix/msg/common.hpp"
#include "rix/msg/component/MessageInfo.hpp"

#include "rix/msg/geometry/Pose2D.hpp"

namespace rix {
namespace msg {
namespace mbot {

#pragma pack(push, 1)
template <uint32_t N>
struct Path2D {
    int64_t utime;
    uint32_t num_poses;
    geometry::Pose2D path[N];

    static inline uint32_t size() { return sizeof(Path2D<N>); }

    static const Path2D* decode(const uint8_t *msg, size_t size) {
        if (size != Path2D::size()) {
            return nullptr;
        }
        return reinterpret_cast<const Path2D*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {12798307682858949031ULL, 7145301262156550826ULL, sizeof(Path2D)};
    }
};
#pragma pack(pop)

} // namespace mbot
} // namespace msg
} // namespace rix
