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
template <uint32_t W, uint32_t H>
struct OccupancyGrid {
    int64_t utime;
    float origin_x;
    float origin_y;
    float meters_per_cell;
    int32_t width;
    int32_t height;
    int32_t num_cells;
    uint8_t cells[W * H];

    static inline uint32_t size() { return sizeof(OccupancyGrid<W, H>); }

    static const OccupancyGrid* decode(const uint8_t *msg, size_t size) {
        if (size != OccupancyGrid::size()) {
            return nullptr;
        }
        return reinterpret_cast<const OccupancyGrid*>(msg);
    }

    const uint8_t* encode() const {
        return reinterpret_cast<const uint8_t*>(this);
    }

    static inline rix::msg::component::MessageInfo info() {
        return {18075691601292310183ULL, 4423470854742770855ULL, sizeof(OccupancyGrid)};
    }
};
#pragma pack(pop)

} // namespace mbot
} // namespace msg
} // namespace rix
