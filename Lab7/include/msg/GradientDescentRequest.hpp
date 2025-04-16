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

class GradientDescentRequest : public MessageBase {
  public:
    double initial_guess;
    double step_scale;
    double tolerance;
    uint32_t max_iterations;

    GradientDescentRequest() = default;
    GradientDescentRequest(const GradientDescentRequest &other) = default;
    ~GradientDescentRequest() = default;

  private:
    size_t size() const override {
        size_t size = 0;
        size += size_base(initial_guess);
        size += size_base(step_scale);
        size += size_base(tolerance);
        size += size_base(max_iterations);
        return size;
    }

    rix::msg::Hash hash() const override {
        return {0x597a67bc77439903ULL, 0xb5a58b915c450f42ULL};
    }

    void serialize(std::vector<uint8_t> &buffer) const override {
        buffer.reserve(buffer.size() + this->size());
        serialize_base(initial_guess, buffer);
        serialize_base(step_scale, buffer);
        serialize_base(tolerance, buffer);
        serialize_base(max_iterations, buffer);
    }

    void deserialize(const std::vector<uint8_t> &buffer, size_t &offset) override {
        deserialize_base(initial_guess, buffer, offset);
        deserialize_base(step_scale, buffer, offset);
        deserialize_base(tolerance, buffer, offset);
        deserialize_base(max_iterations, buffer, offset);
    }
};

} // namespace lab8
} // namespace msg
} // namespace rix