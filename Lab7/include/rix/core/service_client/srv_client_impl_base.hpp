#pragma once

#include <atomic>
#include <map>
#include <memory>
#include <queue>
#include <type_traits>

#include "rix/msg/component/ID.hpp"
#include "rix/msg/component/URI.hpp"

namespace rix {
namespace core {

class SrvClientImplBase {
   public:
    const uint64_t component_id;
    const size_t request_msg_size;
    const size_t response_msg_size;

    SrvClientImplBase(size_t request_msg_size, size_t response_msg_size, uint64_t component_id,
                      rix::msg::component::ID service_id);
    virtual ~SrvClientImplBase();

    void shutdown();
    bool ok() const;

    virtual bool handle_message(const uint8_t *request, uint8_t *response) = 0;
    virtual uint8_t get_protocol() = 0;

   protected:
    rix::msg::component::ID service_id;
    std::atomic<bool> shutdown_flag;
};

using SrvClientImplPtr = std::shared_ptr<SrvClientImplBase>;

}  // namespace core
}  // namespace rix