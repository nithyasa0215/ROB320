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
struct PlannerRequest {
    int64_t utime;
    geometry::Pose2D goal;
    uint8_t require_plan;

    static inline uint32_t size() { return sizeof(PlannerRequest); }

    static const PlannerRequest* decode(const uint8_t *msg, size_t size) {
        if (size != PlannerRequest::size()) {
            return nullptr;
        }
        return reinterpret_cast<const PlannerRequest*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {5182995194601532486ULL, 629875420633125507ULL, sizeof(PlannerRequest)};
    }
};
#pragma pack(pop)

} // namespace mbot
} // namespace msg
} // namespace rix
