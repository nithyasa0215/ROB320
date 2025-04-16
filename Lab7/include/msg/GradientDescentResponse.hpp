#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <cstring>

#include "rix/msg/message_base.hpp"
#include "rix/msg/serializer.hpp"

namespace rix {
namespace msg {
namespace lab8 {

class GradientDescentResponse : public MessageBase {
  public:
    double value;
    bool converged;

    GradientDescentResponse() = default;
    GradientDescentResponse(const GradientDescentResponse &other) = default;
    ~GradientDescentResponse() = default;

  private:
    size_t size() const override {
        size_t size = 0;
        size += size_base(value);
        size += size_base(converged);
        return size;
    }

    rix::msg::Hash hash() const override {
        return {0xb1497151bb152d8aULL, 0x70f0df4a68d980bfULL};
    }

    void serialize(std::vector<uint8_t> &buffer) const override {
        buffer.reserve(buffer.size() + this->size());
        serialize_base(value, buffer);
        serialize_base(converged, buffer);
    }

    void deserialize(const std::vector<uint8_t> &buffer, size_t &offset) override {
        deserialize_base(value, buffer, offset);
        deserialize_base(converged, buffer, offset);
    }
};

} // namespace lab8
} // namespace msg
} // namespace rix