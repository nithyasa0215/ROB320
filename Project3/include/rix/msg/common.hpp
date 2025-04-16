#pragma once

#include <memory>
#include <ostream>
#include <type_traits>

#include "rix/msg/component/MessageInfo.hpp"
#include "rix/msg/component/ComponentInfo.hpp"
#include "rix/util/timing.hpp"

namespace rix {
namespace msg {

template <typename T>
concept RixMsgType = requires(T t, const uint8_t *data, size_t size) {
    { t.encode() } -> std::same_as<const uint8_t *>;
    { T::decode(data, size) } -> std::same_as<const T *>;
    { T::info() } -> std::same_as<rix::msg::component::MessageInfo>;
    { T::size() } -> std::same_as<uint32_t>;
};
#define ASSERT_RIXMSG_TYPE(TMsg) static_assert(rix::msg::RixMsgType<TMsg>, "TMsg is not a rixmsg type");

}  // namespace msg

inline bool operator==(const rix::msg::component::MessageInfo &lhs, const rix::msg::component::MessageInfo &rhs) {
    return lhs.hash_lower == rhs.hash_lower && lhs.hash_upper == rhs.hash_upper && lhs.length == rhs.length;
}

inline bool operator!=(const rix::msg::component::MessageInfo &lhs, const rix::msg::component::MessageInfo &rhs) {
    return !(lhs == rhs);
}
}  // namespace rix